#include "fielddefinitionswindow.h"
#include "ui_fielddefinitionswindow.h"

#include <stdexcept>

#include <QMessageBox>

#include "fielddefinitionwindow.h"
#include "../Controller/fielddefinitionscontroller.h"
#include "../../Components/Controller/componentscontroller.h"
#include "../../Records/Controller/recordscontroller.h"
#include "../../Types/Controller/typescontroller.h"
#include "../../Types/Model/builtintype.h"
#include "../../../Util/listutils.h"

using namespace Tome;


FieldDefinitionsWindow::FieldDefinitionsWindow(FieldDefinitionsController& fieldDefinitionsController,
        ComponentsController& componentsController,
        RecordsController& recordsController,
        TypesController& typesController,
        QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FieldDefinitionsWindow),
    fieldDefinitionsController(fieldDefinitionsController),
    componentsController(componentsController),
    recordsController(recordsController),
    typesController(typesController),
    fieldDefinitionWindow(0)
{
    ui->setupUi(this);

    // Setup view.
    const FieldDefinitionList& fieldDefinitions = this->fieldDefinitionsController.getFieldDefinitionSets()[0].fieldDefinitions;

    this->ui->tableWidget->setRowCount(fieldDefinitions.size());
    this->ui->tableWidget->setColumnCount(6);

    QStringList headers;
    headers << tr("Id");
    headers << tr("Display Name");
    headers << tr("Type");
    headers << tr("Default Value");
    headers << tr("Component");
    headers << tr("Description");
    this->ui->tableWidget->setHorizontalHeaderLabels(headers);

    for (int i = 0; i < fieldDefinitions.size(); ++i)
    {
        this->updateRow(i);
    }

    // Listen for selection changes.
    connect(
                this->ui->tableWidget->selectionModel(),
                SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
                SLOT(tableWidgetSelectionChanged(const QItemSelection &, const QItemSelection &))
                );

    this->updateMenus();
}

FieldDefinitionsWindow::~FieldDefinitionsWindow()
{
    delete this->ui;

    delete this->fieldDefinitionWindow;
}

void FieldDefinitionsWindow::on_actionNew_Field_triggered()
{
    // Setup window.
    if (!this->fieldDefinitionWindow)
    {
        this->fieldDefinitionWindow = new FieldDefinitionWindow(
                    this->fieldDefinitionsController,
                    this->componentsController,
                    this->recordsController,
                    this->typesController,
                    this);
    }

    this->fieldDefinitionWindow->init();

    // Show window.
    int result = this->fieldDefinitionWindow->exec();

    if (result == QDialog::Accepted)
    {
        try
        {
            // Update model.
            FieldDefinition fieldDefinition =
                    this->fieldDefinitionsController.addFieldDefinition(
                        this->fieldDefinitionWindow->getFieldId(),
                        this->fieldDefinitionWindow->getFieldDisplayName(),
                        this->fieldDefinitionWindow->getFieldType(),
                        this->fieldDefinitionWindow->getDefaultValue(),
                        this->fieldDefinitionWindow->getFieldComponent(),
                        this->fieldDefinitionWindow->getFieldDescription());

            // Update view.
            int index = this->fieldDefinitionsController.indexOf(fieldDefinition);

            this->ui->tableWidget->insertRow(index);
            this->updateFieldDefinition(
                        fieldDefinition.id,
                        fieldDefinition.id,
                        fieldDefinition.displayName,
                        fieldDefinition.fieldType,
                        fieldDefinition.defaultValue,
                        fieldDefinition.description,
                        fieldDefinition.component);
        }
        catch (std::out_of_range& e)
        {
            QMessageBox::critical(
                        this,
                        tr("Unable to add field"),
                        e.what(),
                        QMessageBox::Close,
                        QMessageBox::Close);

            this->on_actionNew_Field_triggered();
        }
    }
}

void FieldDefinitionsWindow::on_actionEdit_Field_triggered()
{
    int index = getSelectedFieldIndex();

    if (index < 0)
    {
        return;
    }

    // Get selected field definition.
    const FieldDefinition& fieldDefinition =
            this->fieldDefinitionsController.getFieldDefinitionSets()[0].fieldDefinitions[index];

    // Show window.
    if (!this->fieldDefinitionWindow)
    {
        this->fieldDefinitionWindow = new FieldDefinitionWindow(
                    this->fieldDefinitionsController,
                    this->componentsController,
                    this->recordsController,
                    this->typesController,
                    this);
    }

    this->fieldDefinitionWindow->init();

    // Update view.
    this->fieldDefinitionWindow->setFieldId(fieldDefinition.id);
    this->fieldDefinitionWindow->setFieldDisplayName(fieldDefinition.displayName);
    this->fieldDefinitionWindow->setFieldType(fieldDefinition.fieldType);
    this->fieldDefinitionWindow->setDefaultValue(fieldDefinition.defaultValue);
    this->fieldDefinitionWindow->setFieldDescription(fieldDefinition.description);
    this->fieldDefinitionWindow->setFieldComponent(fieldDefinition.component);

    int result = this->fieldDefinitionWindow->exec();

    if (result == QDialog::Accepted)
    {
        // Update field.
        this->updateFieldDefinition
                (fieldDefinition.id,
                 this->fieldDefinitionWindow->getFieldId(),
                 this->fieldDefinitionWindow->getFieldDisplayName(),
                 this->fieldDefinitionWindow->getFieldType(),
                 this->fieldDefinitionWindow->getDefaultValue(),
                 this->fieldDefinitionWindow->getFieldDescription(),
                 this->fieldDefinitionWindow->getFieldComponent());
    }
}

void FieldDefinitionsWindow::on_actionDelete_Field_triggered()
{
    int index = getSelectedFieldIndex();

    if (index < 0)
    {
        return;
    }

    // Update model.
    this->fieldDefinitionsController.removeFieldDefinitionAt(index);

    // Update view.
    this->ui->tableWidget->removeRow(index);
}

void FieldDefinitionsWindow::on_tableWidget_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index);
    this->on_actionEdit_Field_triggered();
}

void FieldDefinitionsWindow::tableWidgetSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected)
{
    Q_UNUSED(selected);
    Q_UNUSED(deselected);
    this->updateMenus();
}

int FieldDefinitionsWindow::getSelectedFieldIndex() const
{
    return this->ui->tableWidget->currentRow();
}

void FieldDefinitionsWindow::updateMenus()
{
    bool hasSelection = getSelectedFieldIndex() >= 0;

    this->ui->actionEdit_Field->setEnabled(hasSelection);
    this->ui->actionDelete_Field->setEnabled(hasSelection);
}

void FieldDefinitionsWindow::updateFieldDefinition(const QString& oldId, const QString& newId, const QString& displayName, const QString& fieldType, const QVariant& defaultValue, const QString& description, const Component& component)
{
    const FieldDefinition& fieldDefinition = this->fieldDefinitionsController.getFieldDefinition(oldId);

    bool needsSorting = fieldDefinition.displayName != displayName;

    // Update model.
    this->fieldDefinitionsController.updateFieldDefinition(oldId, newId, displayName, fieldType, defaultValue, component, description);

    // Update view.
    int index = this->fieldDefinitionsController.indexOf(fieldDefinition);
    this->updateRow(index);

    // Sort by display name.
    if (needsSorting)
    {
        this->ui->tableWidget->sortItems(1);
    }
}

void FieldDefinitionsWindow::updateRow(const int i)
{
    // Get field definition.
    const FieldDefinitionList& fieldDefinitions = this->fieldDefinitionsController.getFieldDefinitionSets()[0].fieldDefinitions;
    const FieldDefinition& fieldDefinition = fieldDefinitions[i];

    // Convert default value to string.
    QString defaultValueString = fieldDefinition.defaultValue.toString();

    if (this->typesController.isCustomType(fieldDefinition.fieldType))
    {
        const CustomType& customType = this->typesController.getCustomType(fieldDefinition.fieldType);

        if (customType.isList())
        {
            defaultValueString = toString(fieldDefinition.defaultValue.toList());
        }
    }

    this->ui->tableWidget->setItem(i, 0, new QTableWidgetItem(fieldDefinition.id));
    this->ui->tableWidget->setItem(i, 1, new QTableWidgetItem(fieldDefinition.displayName));
    this->ui->tableWidget->setItem(i, 2, new QTableWidgetItem(fieldDefinition.fieldType));
    this->ui->tableWidget->setItem(i, 3, new QTableWidgetItem(defaultValueString));
    this->ui->tableWidget->setItem(i, 4, new QTableWidgetItem(fieldDefinition.component));
    this->ui->tableWidget->setItem(i, 5, new QTableWidgetItem(fieldDefinition.description));

    if (fieldDefinition.fieldType == BuiltInType::Color)
    {
        // Show color preview.
        QColor color = fieldDefinition.defaultValue.value<QColor>();
        this->ui->tableWidget->item(i, 3)->setData(Qt::DecorationRole, color);
    }
}
