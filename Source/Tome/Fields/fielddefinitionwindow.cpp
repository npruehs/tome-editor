#include "fielddefinitionwindow.h"
#include "ui_fielddefinitionwindow.h"

#include <QMessageBox>

#include "../Types/builtintype.h"
#include "../Values/valueconverter.h"

using namespace Tome;


FieldDefinitionWindow::FieldDefinitionWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FieldDefinitionWindow)
{
    ui->setupUi(this);

    // Add widget for specifying the default field value.
    this->fieldValueWidget = new FieldValueWidget(this);
    QFormLayout* layout = static_cast<QFormLayout*>(this->layout());
    layout->insertRow(2, tr("Default Value:"), this->fieldValueWidget);
}

FieldDefinitionWindow::~FieldDefinitionWindow()
{
    delete ui;
}

void FieldDefinitionWindow::accept()
{
    // Validate data.
    if (this->validate())
    {
        this->done(Accepted);
    }
}

QString FieldDefinitionWindow::getFieldComponent() const
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

void FieldDefinitionWindow::setProject(QSharedPointer<Project> project)
{
    this->project = project;

    // Set component names.
    this->ui->comboBoxComponent->clear();
    this->ui->comboBoxComponent->addItem(QString());
    this->ui->comboBoxComponent->addItems(project->getComponentNames());

    // Set type names.
    this->ui->comboBoxType->clear();

    QStringList typeNames = this->project->getTypeNames();

    for (int i = 0; i < typeNames.length(); ++i)
    {
        this->ui->comboBoxType->addItem(typeNames.at(i));
    }
}

void FieldDefinitionWindow::on_comboBoxType_currentIndexChanged(const QString &fieldType)
{
    if (fieldType == BuiltInType::Reference)
    {
        QStringList recordNames = this->project->getRecordNames();

        // Allow clearing the field.
        recordNames << QString();

        this->fieldValueWidget->setEnumeration(recordNames);
        this->fieldValueWidget->setFieldType(fieldType);
    }
    else
    {
        QSharedPointer<CustomType> type = this->project->getCustomType(fieldType);

        if (type != 0)
        {
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
