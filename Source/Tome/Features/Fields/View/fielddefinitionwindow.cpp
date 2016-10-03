#include "fielddefinitionwindow.h"
#include "ui_fielddefinitionwindow.h"

#include <QMessageBox>

#include "fieldvaluewidget.h"
#include "../Controller/fielddefinitionscontroller.h"
#include "../../Components/Controller/componentscontroller.h"
#include "../../Facets/Controller/facet.h"
#include "../../Facets/Controller/facetscontroller.h"
#include "../../Records/Controller/recordscontroller.h"
#include "../../Types/Controller/typescontroller.h"
#include "../../Types/Model/builtintype.h"
#include "../../../Util/stringutils.h"

using namespace Tome;

const int FieldDefinitionWindow::DefaultFormRows = 7;
const int FieldDefinitionWindow::FacetFormRow = 6;
const int FieldDefinitionWindow::ValueFormRow = 3;


FieldDefinitionWindow::FieldDefinitionWindow(
        FieldDefinitionsController& fieldDefinitionsController,
        ComponentsController& componentsController,
        RecordsController& recordsController,
        TypesController& typesController,
        FacetsController& facetsController,
        QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FieldDefinitionWindow),
    fieldDefinitionsController(fieldDefinitionsController),
    componentsController(componentsController),
    recordsController(recordsController),
    typesController(typesController),
    facetsController(facetsController)
{
    ui->setupUi(this);

    // Add widget for specifying the default field value.
    this->fieldValueWidget = new FieldValueWidget(this->recordsController, this->typesController, this);
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

QVariantMap FieldDefinitionWindow::getFieldFacets() const
{
    QVariantMap facetMap;

    // Get all facets registered for the current type.
    QList<Facet*> typeFacets = this->facetsController.getFacets(this->getFieldType());

    for (int i = 0; i < typeFacets.count(); ++i)
    {
        // Get facet.
        Facet* facet = typeFacets[i];

        // Get current facet value.
        QString key = facet->getKey();
        QVariant value = facet->getWidgetValue(this->facetWidgets[i]);

        // Insert into facets map.
        facetMap.insert(key, value);
    }

    return facetMap;
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

void FieldDefinitionWindow::setFieldFacets(const QVariantMap& facets)
{
    // Remove all existing facet widget from previous times this window was shown.
    QFormLayout* layout = static_cast<QFormLayout*>(this->layout());

    while (!this->facetWidgets.empty())
    {
        QWidget* facetWidget = this->facetWidgets.takeAt(0);
        QWidget* facetLabel = layout->labelForField(facetWidget);
        layout->removeWidget(facetLabel);
        layout->removeWidget(facetWidget);
        delete facetLabel;
        delete facetWidget;
    }

    // Add new facet widgets.
    QList<Facet*> typeFacets = this->facetsController.getFacets(this->getFieldType());

    for (int i = 0; i < typeFacets.count(); ++i)
    {
        // Create facet widget and label.
        Facet* facet = typeFacets[i];
        QString facetDisplayName = facet->getDisplayName();
        QWidget* facetWidget = facet->createWidget();

        // Get current facet value from field definition.
        QString facetKey = facet->getKey();
        QVariant facetValue = facets[facetKey];
        facet->setWidgetValue(facetWidget, facetValue);

        // Insert into layout and remember for later removal.
        layout->insertRow(FacetFormRow + i, facetDisplayName + ":", facetWidget);
        this->facetWidgets.push_back(facetWidget);
    }
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

    return true;
}
