#include "fielddefinitionwindow.h"
#include "ui_fielddefinitionwindow.h"

#include <QMessageBox>

#include "fieldvaluewidget.h"
#include "../Controller/fielddefinitionscontroller.h"
#include "../../Components/Controller/componentscontroller.h"
#include "../../Facets/Controller/facet.h"
#include "../../Facets/Controller/facetscontroller.h"
#include "../../Facets/Model/facetcontext.h"
#include "../../Records/Controller/recordscontroller.h"
#include "../../Types/Controller/typescontroller.h"
#include "../../Types/Model/builtintype.h"
#include "../../../Util/stringutils.h"

using namespace Tome;

const int FieldDefinitionWindow::DefaultFormRows = 7;
const int FieldDefinitionWindow::ValueFormRow = 3;


FieldDefinitionWindow::FieldDefinitionWindow(FieldDefinitionsController& fieldDefinitionsController,
        ComponentsController& componentsController,
        FacetsController& facetsController,
        ProjectController& projectController,
        RecordsController& recordsController,
        TypesController& typesController,
        QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FieldDefinitionWindow),
    fieldDefinitionsController(fieldDefinitionsController),
    componentsController(componentsController),
    facetsController(facetsController),
    projectController(projectController),
    recordsController(recordsController),
    typesController(typesController)
{
    ui->setupUi(this);

    // Add widget for specifying the default field value.
    this->fieldValueWidget = new FieldValueWidget(this->facetsController, this->projectController, this->recordsController, this->typesController, this);
    QFormLayout* layout = static_cast<QFormLayout*>(this->layout());
    layout->insertRow(ValueFormRow, tr("Default Value:"), this->fieldValueWidget);
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

    // Adjust size, so opening a window with large content
    // (e.g. color picker) does not cause huge empty areas in subsequent
    // windows with small content (e.g. checkbox).
    this->adjustSize();
}

Component FieldDefinitionWindow::getFieldComponent() const
{
    return this->ui->comboBoxComponent->currentText();
}

QString FieldDefinitionWindow::getFieldDefinitionSetName() const
{
    return this->ui->comboBoxFieldDefinitionSet->currentText();
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

    // Set field definition set names.
    const QStringList fieldDefinitionSetNames = this->fieldDefinitionsController.getFieldDefinitionSetNames();
    this->setFieldDefinitionSetNames(fieldDefinitionSetNames);
    this->setFieldDefinitionSetName(fieldDefinitionSetNames.first());
}

void FieldDefinitionWindow::setFieldComponent(const QString& component) const
{
    this->ui->comboBoxComponent->setCurrentText(component);
}

void FieldDefinitionWindow::setFieldDefinitionSetName(const QString& fieldDefinitionSetName)
{
    this->ui->comboBoxFieldDefinitionSet->setCurrentText(fieldDefinitionSetName);
}

void FieldDefinitionWindow::setFieldDefinitionSetNames(const QStringList& fieldDefinitionSetNames)
{
    this->ui->comboBoxFieldDefinitionSet->clear();
    this->ui->comboBoxFieldDefinitionSet->addItems(fieldDefinitionSetNames);
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

    // Id must not contain any reserved characters.
    if (this->getFieldId().contains("$"))
    {
        QMessageBox::information(
                    this,
                    tr("Reserved character"),
                    tr("Please specify an id without $ character."),
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

    // Default value must be valid.
    QString validationError = this->fieldValueWidget->validate();

    if (!validationError.isEmpty())
    {
        QMessageBox::information(
                    this,
                    tr("Invalid data"),
                    validationError,
                    QMessageBox::Close,
                    QMessageBox::Close);
        return false;
    }

    return true;
}
