#include "fielddefinitionwindow.h"
#include "ui_fielddefinitionwindow.h"

#include <QMessageBox>

#include "../../Types/Model/builtintype.h"

using namespace Tome;


FieldDefinitionWindow::FieldDefinitionWindow(
        FieldDefinitionsController& fieldDefinitionsController,
        ComponentsController& componentsController,
        RecordsController& recordsController,
        TypesController& typesController,
        QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FieldDefinitionWindow),
    fieldDefinitionsController(fieldDefinitionsController),
    componentsController(componentsController),
    recordsController(recordsController),
    typesController(typesController)
{
    ui->setupUi(this);

    // Add widget for specifying the default field value.
    this->fieldValueWidget = new FieldValueWidget(this);
    QFormLayout* layout = static_cast<QFormLayout*>(this->layout());
    layout->insertRow(2, tr("Default Value:"), this->fieldValueWidget);
}

FieldDefinitionWindow::~FieldDefinitionWindow()
{
    delete this->ui;

    delete this->fieldValueWidget;
}

void FieldDefinitionWindow::accept()
{
    // Validate data.
    if (this->validate())
    {
        this->done(Accepted);
    }
}

int FieldDefinitionWindow::exec()
{
    // Set component names.
    this->ui->comboBoxComponent->clear();
    this->ui->comboBoxComponent->addItem(QString());
    this->ui->comboBoxComponent->addItems(this->componentsController.getComponents());

    // Set type names.
    this->ui->comboBoxType->clear();

    const QStringList& typeNames = this->typesController.getTypeNames();

    for (int i = 0; i < typeNames.length(); ++i)
    {
        this->ui->comboBoxType->addItem(typeNames[i]);
    }

    // Show dialog.
    return QDialog::exec();
}

Component FieldDefinitionWindow::getFieldComponent() const
{
    return this->ui->comboBoxComponent->currentText();
}

QString FieldDefinitionWindow::getFieldDescription() const
{
    return this->ui->plainTextEditDescription->toPlainText();
}

QString FieldDefinitionWindow::getFieldDisplayName() const
{
    return this->ui->lineEditDisplayName->text();
}

QString FieldDefinitionWindow::getFieldId() const
{
    return this->ui->lineEditId->text();
}

QString FieldDefinitionWindow::getDefaultValue() const
{
    return this->fieldValueWidget->getFieldValue();
}

QString FieldDefinitionWindow::getFieldType() const
{
    return this->fieldValueWidget->getFieldType();
}

void FieldDefinitionWindow::setFieldComponent(const QString& component) const
{
    this->ui->comboBoxComponent->setCurrentText(component);
}

void FieldDefinitionWindow::setFieldDescription(const QString& description)
{
    this->ui->plainTextEditDescription->setPlainText(description);
}

void FieldDefinitionWindow::setFieldDisplayName(const QString& displayName)
{
    this->ui->lineEditDisplayName->setText(displayName);
}

void FieldDefinitionWindow::setFieldId(const QString& fieldId)
{
    this->ui->lineEditId->setText(fieldId);
}

void FieldDefinitionWindow::setDefaultValue(const QString& defaultValue)
{
    this->fieldValueWidget->setFieldValue(defaultValue);
}

void FieldDefinitionWindow::setFieldType(const QString& fieldType) const
{
    this->ui->comboBoxType->setCurrentText(fieldType);
}

void FieldDefinitionWindow::on_comboBoxType_currentIndexChanged(const QString &fieldType)
{
    if (fieldType == BuiltInType::Reference)
    {
        QStringList recordNames = this->recordsController.getRecordNames();

        // Allow clearing the field.
        recordNames << QString();

        this->fieldValueWidget->setEnumeration(recordNames);
        this->fieldValueWidget->setFieldType(fieldType);
    }
    else
    {
        const bool isCustomType = this->typesController.isCustomType(fieldType);

        if (isCustomType)
        {
            const CustomType& type = this->typesController.getCustomType(fieldType);
            this->fieldValueWidget->setCustomFieldType(type);
        }
        else
        {
            // Default built-in type.
            this->fieldValueWidget->setFieldType(fieldType);
        }
    }
}

void FieldDefinitionWindow::on_lineEditDisplayName_textEdited(const QString &displayName)
{
    this->setFieldId(displayName);
}

bool FieldDefinitionWindow::validate()
{
    // Id must not be empty.
    if (this->getFieldId().isEmpty())
    {
        QMessageBox::information(
                    this,
                    tr("Missing data"),
                    tr("Please specify an id for the field."),
                    QMessageBox::Close,
                    QMessageBox::Close);
        return false;
    }

    // Display name must not be empty.
    if (this->getFieldDisplayName().isEmpty())
    {
        QMessageBox::information(
                    this,
                    tr("Missing data"),
                    tr("Please specify a name for the field."),
                    QMessageBox::Close,
                    QMessageBox::Close);
        return false;
    }

    return true;
}
