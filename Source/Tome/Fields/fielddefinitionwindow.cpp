#include "fielddefinitionwindow.h"
#include "ui_fielddefinitionwindow.h"

#include <QMessageBox>

#include "../Fields/fieldtype.h"
#include "../Values/valueconverter.h"

using namespace Tome;


FieldDefinitionWindow::FieldDefinitionWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FieldDefinitionWindow)
{
    ui->setupUi(this);

    // Add supported field types.
    this->ui->comboBoxType->addItem("Int");
    this->ui->comboBoxType->addItem("String");
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
    switch (this->getFieldType())
    {
        case FieldType::Int:
        case FieldType::String:
            return this->ui->lineEditDefaultValue->text();
            break;
    }

    return QString();
}

FieldType::FieldType FieldDefinitionWindow::getFieldType() const
{
    const QString fieldType = this->ui->comboBoxType->currentText();
    ValueConverter valueConverter;
    return valueConverter.StringToFieldType(fieldType);
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
    switch (this->getFieldType())
    {
        case FieldType::Int:
        case FieldType::String:
            this->ui->lineEditDefaultValue->setText(defaultValue);
            break;
    }
}

void FieldDefinitionWindow::setFieldType(const FieldType::FieldType& fieldType) const
{
    ValueConverter valueConverter;
    const QString fieldTypeName = valueConverter.FieldTypeToString(fieldType);
    this->ui->comboBoxType->setCurrentText(fieldTypeName);
}

void FieldDefinitionWindow::on_comboBoxType_currentIndexChanged(const QString &fieldType)
{
    ValueConverter valueConverter;
    FieldType::FieldType newType = valueConverter.StringToFieldType(fieldType);

    switch (newType)
    {
        case FieldType::Int:
        case FieldType::String:
            this->ui->lineEditDefaultValue->setVisible(true);
            break;

        default:
            this->ui->lineEditDefaultValue->setVisible(false);
            break;
    }
}

void FieldDefinitionWindow::on_lineEditDisplayName_textEdited(const QString &displayName)
{
    this->ui->lineEditId->setText(displayName);
}

bool FieldDefinitionWindow::validate()
{
    // Id must not be empty.
    if (this->getFieldId().size() == 0)
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
    if (this->getFieldDisplayName().size() == 0)
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
