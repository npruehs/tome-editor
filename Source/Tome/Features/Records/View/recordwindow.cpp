#include "recordwindow.h"
#include "ui_recordwindow.h"

#include <QCheckBox>
#include <QMessageBox>

#include "../../../Util/stringutils.h"


using namespace Tome;

const QString RecordWindow::PropertyFieldComponent = "FieldComponent";
const QString RecordWindow::PropertyFieldId = "FieldId";
const QString RecordWindow::PropertyComponentId = "ComponentId";


RecordWindow::RecordWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RecordWindow)
{
    ui->setupUi(this);

    this->ui->lineEditId->setValidator(&nameValidator);
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

void RecordWindow::showEvent(QShowEvent* event)
{
    QDialog::showEvent(event);
    this->ui->lineEditDisplayName->setFocus();
}

QString RecordWindow::getRecordDisplayName() const
{
    return this->ui->lineEditDisplayName->text();
}

QString RecordWindow::getRecordEditorIconFieldId() const
{
    return this->ui->comboBoxEditorIconField->currentData().toString();
}

QVariant RecordWindow::getRecordId() const
{
    if (this->recordIdType == RecordIdType::String)
    {
        return this->ui->lineEditId->text();
    }

    return this->recordId;
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

QString RecordWindow::getRecordSetName() const
{
    return this->ui->comboBoxRecordSet->currentText();
}

void RecordWindow::setDisallowedRecordIds(const QVariantList disallowedRecordIds)
{
    this->disallowedRecordIds = disallowedRecordIds;
}

void RecordWindow::setRecordDisplayName(const QString& displayName)
{
    this->ui->lineEditDisplayName->setText(displayName);
}

void RecordWindow::setRecordId(const QVariant& id)
{
    this->recordId = id;

    if (this->recordIdType == RecordIdType::String)
    {
        QString recordIdString = id.isNull() ? QString() : id.toString();
        this->ui->lineEditId->setText(recordIdString);
    }
    else
    {
        QString recordIdString = id.isNull() ? tr("Unassigned") : id.toString();
        this->ui->labelId->setText(recordIdString);
    }
}

void RecordWindow::setRecordIdLocked(const bool recordIdLocked)
{
    this->recordIdLocked = recordIdLocked;
}

void RecordWindow::setRecordIdType(const RecordIdType::RecordIdType recordIdType)
{
    this->recordIdType = recordIdType;

    switch (recordIdType)
    {
        case RecordIdType::Integer:
        case RecordIdType::Uuid:
        case RecordIdType::Invalid:
            this->ui->labelIdLineEdit->hide();
            this->ui->lineEditId->hide();

            this->ui->labelIdLabel->show();
            this->ui->labelId->show();
            break;

        case RecordIdType::String:
            this->ui->labelIdLineEdit->show();
            this->ui->lineEditId->show();

            this->ui->labelIdLabel->hide();
            this->ui->labelId->hide();
    }
}

void RecordWindow::setRecordEditorIconFieldId(const QString& editorIconFieldId)
{
    for (int i = 0; i < this->ui->comboBoxEditorIconField->count(); ++i)
    {
        if (this->ui->comboBoxEditorIconField->itemData(i).toString() == editorIconFieldId)
        {
            this->ui->comboBoxEditorIconField->setCurrentIndex(i);
            return;
        }
    }
}

void RecordWindow::setRecordFields(const FieldDefinitionList& fieldDefinitions)
{
    // Clear current fields.
    this->clearRecordFields();

    // Add all passed fields.
    this->ui->comboBoxEditorIconField->addItem(QString(), QString());

    for (int i = 0; i < fieldDefinitions.size(); ++i)
    {
        const FieldDefinition& fieldDefinition = fieldDefinitions.at(i);
        this->setRecordField(fieldDefinition.id, fieldDefinition.component, RecordFieldState::Disabled);
        this->ui->comboBoxEditorIconField->addItem(fieldDefinition.displayName, fieldDefinition.id);
    }
}

void RecordWindow::setRecordFields(const FieldDefinitionList& fieldDefinitions, const ComponentList &componentDefinitions, const RecordFieldValueMap& ownFieldValues, const RecordFieldValueMap& inheritedFieldValues)
{
    // Clear current fields.
    this->clearRecordFields();

    // Add all components.
    setRecordComponents( componentDefinitions );

    // Add all passed fields.
    this->ui->comboBoxEditorIconField->addItem(QString(), QString());

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
        this->ui->comboBoxEditorIconField->addItem(fieldDefinition.displayName, fieldDefinition.id);

        // Modify state of component checkboxes based on fieldState
        for (int i = 0; i < this->ui->scrollAreaComponentsContents->layout()->count(); ++i)
        {
            QCheckBox* component_cb = static_cast<QCheckBox*>(this->ui->scrollAreaComponentsContents->layout()->itemAt(i)->widget());
            QString component_id = component_cb->property(PropertyComponentId.toStdString().c_str()).toString();
            if (fieldDefinition.component == component_id)
            {
                if ( RecordFieldState::InheritedEnabled == fieldState )
                {
                    component_cb->setCheckState( Qt::CheckState::Checked );
                    component_cb->setEnabled( false );
                }
                else
                {
                    component_cb->setCheckState( RecordFieldState::Enabled == fieldState ? Qt::CheckState::Checked : Qt::CheckState::Unchecked );
                }
                break;
            }
        }
    }
}

void RecordWindow::setRecordComponents(const Tome::ComponentList& components)
{
    // Clear current fields.
    this->clearRecordComponents();

    // Add all passed fields.
    for (int i = 0; i < components.size(); ++i)
    {
        const Component& component = components.at(i);
        this->setRecordComponent(component, RecordFieldState::Disabled);
    }
}

void RecordWindow::setRecordSetName(const QString& recordSetName)
{
    this->ui->comboBoxRecordSet->setCurrentText(recordSetName);
}

void RecordWindow::setRecordSetNames(const QStringList& recordSetNames)
{
    this->ui->comboBoxRecordSet->clear();
    this->ui->comboBoxRecordSet->addItems(recordSetNames);
}

void RecordWindow::on_lineEditDisplayName_textEdited(const QString& displayName)
{
    // If the user manually changed the record id we must not change it when the display name changes.
    if (this->recordIdType == RecordIdType::String && !recordIdLocked)
    {
        this->setRecordId(stripWhitespaces(displayName));
    }
}

void RecordWindow::on_lineEditId_textEdited(const QString &arg1)
{
    // If the user manually changed the record id we must not change it when the display name changes.
    recordIdLocked = !arg1.isEmpty();
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

    // Apply state to component checkbox of fieldComponent type.
    for (int i = 0; i < this->ui->scrollAreaComponentsContents->layout()->count(); ++i)
    {
        QCheckBox* component_cb = static_cast<QCheckBox*>(this->ui->scrollAreaComponentsContents->layout()->itemAt(i)->widget());
        QString component_id = component_cb->property(PropertyComponentId.toStdString().c_str()).toString();
        if (fieldComponent == component_id)
        {
            component_cb->setCheckState((Qt::CheckState)state);
            break;
        }
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

void RecordWindow::onComponentCheckBoxStateChanged(int state)
{
    // Get field id.
    QObject* checkbox = sender();
    QString componentId = checkbox->property(PropertyComponentId.toStdString().c_str()).toString();

    // Apply state to all checkboxes of same field component.
    for (int i = 0; i < this->ui->scrollAreaFieldsContents->layout()->count(); ++i)
    {
        QCheckBox* otherCheckBox = static_cast<QCheckBox*>(this->ui->scrollAreaFieldsContents->layout()->itemAt(i)->widget());
        QString otherFieldComponent = otherCheckBox->property(PropertyFieldComponent.toStdString().c_str()).toString();

        if (otherFieldComponent == componentId)
        {
            otherCheckBox->setCheckState((Qt::CheckState)state);
        }
    }
}

void RecordWindow::clearRecordComponents()
{
    while (!this->ui->scrollAreaComponentsContents->layout()->isEmpty())
    {
        QLayoutItem* item = this->ui->scrollAreaComponentsContents->layout()->takeAt(0);
        delete item->widget();
        delete item;
    }
}

void RecordWindow::clearRecordFields()
{
    while (!this->ui->scrollAreaFieldsContents->layout()->isEmpty())
    {
        QLayoutItem* item = this->ui->scrollAreaFieldsContents->layout()->takeAt(0);
        delete item->widget();
        delete item;
    }

    this->ui->comboBoxEditorIconField->clear();
}

void RecordWindow::setRecordComponent(const QString& componentId, const Tome::RecordFieldState::RecordFieldState state)
{
    // Build check box text.
    QString checkBoxText = componentId;

    // Create checkbox.
    QCheckBox* checkBox = new QCheckBox(checkBoxText);
    checkBox->setProperty(PropertyComponentId.toStdString().c_str(), componentId);

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
    connect(checkBox, SIGNAL(stateChanged(int)), this, SLOT(onComponentCheckBoxStateChanged(int)) );

    // Add to layout.
    this->ui->scrollAreaComponentsContents->layout()->addWidget(checkBox);
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

bool RecordWindow::validate()
{
    // Id must not be empty.
    if (this->recordIdType == RecordIdType::String && this->getRecordId().toString().isEmpty())
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

    // Record ids must be unique.
    if (this->disallowedRecordIds.contains(this->getRecordId()))
    {
        QMessageBox::information(
                    this,
                    tr("Duplicate record id"),
                    tr("Please specify another id for the record."),
                    QMessageBox::Close,
                    QMessageBox::Close);
        return false;
    }

    return true;
}
