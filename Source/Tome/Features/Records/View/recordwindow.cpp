#include "recordwindow.h"
#include "ui_recordwindow.h"

#include <QCheckBox>
#include <QMessageBox>

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

QMap<QString, bool> RecordWindow::getRecordFields() const
{
    QMap<QString, bool> fields;

    for (int i = 0; i < this->ui->scrollAreaFieldsContents->layout()->count(); ++i)
    {
        QLayoutItem* item = this->ui->scrollAreaFieldsContents->layout()->itemAt(i);
        QCheckBox* checkBox = static_cast<QCheckBox*>(item->widget());

        QString fieldId = checkBox->property(PropertyFieldId.toStdString().c_str()).toString();
        bool fieldEnabled = checkBox->isChecked();

        fields.insert(fieldId, fieldEnabled);
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

void RecordWindow::setRecordField(const QString& fieldId, const QString& fieldComponent, const bool enabled)
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
    checkBox->setChecked(enabled);

    // Connect to signal.
    connect(checkBox, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged(int)) );

    // Add to layout.
    this->ui->scrollAreaFieldsContents->layout()->addWidget(checkBox);
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
