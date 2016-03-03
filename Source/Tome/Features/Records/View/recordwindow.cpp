#include "recordwindow.h"
#include "ui_recordwindow.h"

#include <QCheckBox>
#include <QMessageBox>

using namespace Tome;

const QString RecordWindow::PropertyFieldComponent = "FieldComponent";
const QString RecordWindow::PropertyFieldId = "FieldId";


RecordWindow::RecordWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RecordWindow)
{
    ui->setupUi(this);
}

RecordWindow::~RecordWindow()
{
    delete this->ui;
}

void RecordWindow::accept()
{
    // Validate data.
    if (this->validate())
    {
        this->done(Accepted);
    }
}

QString RecordWindow::getRecordDisplayName() const
{
    return this->ui->lineEditDisplayName->text();
}

QString RecordWindow::getRecordId() const
{
    return this->ui->lineEditId->text();
}

QMap<QString, RecordFieldState::RecordFieldState> RecordWindow::getRecordFields() const
{
    QMap<QString, RecordFieldState::RecordFieldState> fields;

    for (int i = 0; i < this->ui->scrollAreaFieldsContents->layout()->count(); ++i)
    {
        QLayoutItem* item = this->ui->scrollAreaFieldsContents->layout()->itemAt(i);
        QCheckBox* checkBox = static_cast<QCheckBox*>(item->widget());

        QString fieldId = checkBox->property(PropertyFieldId.toStdString().c_str()).toString();

        RecordFieldState::RecordFieldState fieldState;

        if (!checkBox->isEnabled())
        {
            fieldState = RecordFieldState::InheritedEnabled;
        }
        else if (checkBox->isChecked())
        {
            fieldState = RecordFieldState::Enabled;
        }
        else
        {
            fieldState = RecordFieldState::Disabled;
        }

        fields.insert(fieldId, fieldState);
    }

    return fields;
}

void RecordWindow::clearRecordFields()
{
    while (!this->ui->scrollAreaFieldsContents->layout()->isEmpty())
    {
        QLayoutItem* item = this->ui->scrollAreaFieldsContents->layout()->takeAt(0);
        delete item->widget();
        delete item;
    }
}

void RecordWindow::setRecordDisplayName(const QString& displayName)
{
    this->ui->lineEditDisplayName->setText(displayName);
}

void RecordWindow::setRecordId(const QString& id)
{
    this->ui->lineEditId->setText(id);
}

void RecordWindow::setRecordField(const QString& fieldId, const QString& fieldComponent, const RecordFieldState::RecordFieldState state)
{
    // Build check box text.
    QString checkBoxText = fieldId;
    if (!fieldComponent.isEmpty())
    {
        checkBoxText.append(" (" + fieldComponent + ")");
    }

    // Create checkbox.
    QCheckBox* checkBox = new QCheckBox(checkBoxText);
    checkBox->setProperty(PropertyFieldId.toStdString().c_str(), fieldId);
    checkBox->setProperty(PropertyFieldComponent.toStdString().c_str(), fieldComponent);

    // Setup checkbox.
    if (state == RecordFieldState::Enabled || state == RecordFieldState::InheritedEnabled)
    {
        checkBox->setChecked(true);
    }

    if (state == RecordFieldState::InheritedEnabled)
    {
        checkBox->setEnabled(false);
        checkBox->setToolTip(tr("This field is inherited."));
    }

    // Connect to signal.
    connect(checkBox, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged(int)) );

    // Add to layout.
    this->ui->scrollAreaFieldsContents->layout()->addWidget(checkBox);
}

void RecordWindow::setRecordFields(const FieldDefinitionList& fieldDefinitions)
{
    // Clear current fields.
    this->clearRecordFields();

    // Add all passed fields.
    for (int i = 0; i < fieldDefinitions.size(); ++i)
    {
        const FieldDefinition& fieldDefinition = fieldDefinitions.at(i);
        this->setRecordField(fieldDefinition.id, fieldDefinition.component, RecordFieldState::Disabled);
    }
}

void RecordWindow::setRecordFields(const FieldDefinitionList& fieldDefinitions, const RecordFieldValueMap& ownFieldValues, const RecordFieldValueMap& inheritedFieldValues)
{
    // Clear current fields.
    this->clearRecordFields();

    // Add all passed fields.
    for (int i = 0; i < fieldDefinitions.size(); ++i)
    {
        const FieldDefinition& fieldDefinition = fieldDefinitions.at(i);
        RecordFieldState::RecordFieldState fieldState = RecordFieldState::Disabled;

        // Check if any parent contains field.
        if (inheritedFieldValues.contains(fieldDefinition.id))
        {
            fieldState = RecordFieldState::InheritedEnabled;
        }
        // Check if record itself contains field.
        else if (ownFieldValues.contains(fieldDefinition.id))
        {
            fieldState = RecordFieldState::Enabled;
        }

        // Add to view.
        this->setRecordField(fieldDefinition.id, fieldDefinition.component, fieldState);
    }
}

void RecordWindow::on_lineEditDisplayName_textEdited(const QString& displayName)
{
    this->setRecordId(displayName);
}

void RecordWindow::onCheckBoxStateChanged(int state)
{
    // Get field id.
    QObject* checkbox = sender();
    QString fieldComponent = checkbox->property(PropertyFieldComponent.toStdString().c_str()).toString();

    if (fieldComponent.isEmpty())
    {
        return;
    }

    // Apply state to all checkboxes of same field component.
    for (int i = 0; i < this->ui->scrollAreaFieldsContents->layout()->count(); ++i)
    {
        QCheckBox* otherCheckBox = static_cast<QCheckBox*>(this->ui->scrollAreaFieldsContents->layout()->itemAt(i)->widget());
        QString otherFieldComponent = otherCheckBox->property(PropertyFieldComponent.toStdString().c_str()).toString();

        if (otherFieldComponent == fieldComponent)
        {
            otherCheckBox->setCheckState((Qt::CheckState)state);
        }
    }
}

bool RecordWindow::validate()
{
    // Id must not be empty.
    if (this->getRecordId().size() == 0)
    {
        QMessageBox::information(
                    this,
                    tr("Missing data"),
                    tr("Please specify an id for the record."),
                    QMessageBox::Close,
                    QMessageBox::Close);
        return false;
    }

    // Display name must not be empty.
    if (this->getRecordDisplayName().size() == 0)
    {
        QMessageBox::information(
                    this,
                    tr("Missing data"),
                    tr("Please specify a name for the record."),
                    QMessageBox::Close,
                    QMessageBox::Close);
        return false;
    }

    return true;
}
