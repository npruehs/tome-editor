#include "fielddefinitionwindow.h"
#include "ui_fielddefinitionwindow.h"

#include <QMessageBox>

#include "fieldvaluewidget.h"
#include "../Controller/fielddefinitionscontroller.h"
#include "../../Components/Controller/componentscontroller.h"
#include "../../Records/Controller/recordscontroller.h"
#include "../../Types/Controller/typescontroller.h"
#include "../../Types/Model/builtintype.h"
#include "../../../Util/stringutils.h"

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
    this->fieldValueWidget = new FieldValueWidget(this->recordsController, this->typesController, this);
    QFormLayout* layout = static_cast<QFormLayout*>(this->layout());
    layout->insertRow(3, tr("Default Value:"), this->fieldValueWidget);
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

void FieldDefinitionWindow::showEvent(QShowEvent* event)
{
    QDialog::showEvent(event);
    this->ui->lineEditDisplayName->setFocus();
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

QVariant FieldDefinitionWindow::getDefaultValue() const
{
    return this->fieldValueWidget->getFieldValue();
}

QString FieldDefinitionWindow::getFieldType() const
{
    return this->fieldValueWidget->getFieldType();
}

void FieldDefinitionWindow::init()
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

void FieldDefinitionWindow::setDefaultValue(const QVariant& defaultValue)
{
    this->fieldValueWidget->setFieldValue(defaultValue);
}

void FieldDefinitionWindow::setFieldType(const QString& fieldType) const
{
    this->ui->comboBoxType->setCurrentText(fieldType);
}

void FieldDefinitionWindow::on_comboBoxType_currentIndexChanged(const QString &fieldType)
{
    this->fieldValueWidget->setFieldType(fieldType);
}

void FieldDefinitionWindow::on_lineEditDisplayName_textEdited(const QString &displayName)
{
    this->setFieldId(stripWhitespaces(displayName));
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

    // Id must not contain any whitespaces.
    if (containsWhitespaces(this->getFieldId()))
    {
        QMessageBox::information(
                    this,
                    tr("Incorrect id"),
                    tr("Please specify an id without whitespaces."),
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
