#include "fielddefinitionswindow.h"
#include "ui_fielddefinitionswindow.h"

#include <stdexcept>

#include <QMessageBox>

#include "fielddefinitionwindow.h"
#include "../Controller/fielddefinitionscontroller.h"
#include "../../Components/Controller/componentscontroller.h"
#include "../../Records/Controller/recordscontroller.h"
#include "../../Search/Controller/findusagescontroller.h"
#include "../../Types/Controller/typescontroller.h"
#include "../../Types/Model/builtintype.h"
#include "../../../Util/listutils.h"

using namespace Tome;


FieldDefinitionsWindow::FieldDefinitionsWindow(FieldDefinitionsController& fieldDefinitionsController,
        ComponentsController& componentsController,
        RecordsController& recordsController,
        TypesController& typesController,
        FindUsagesController& findUsagesController,
        QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FieldDefinitionsWindow),
    fieldDefinitionsController(fieldDefinitionsController),
    componentsController(componentsController),
    recordsController(recordsController),
    typesController(typesController),
    findUsagesController(findUsagesController),
    fieldDefinitionWindow(0)
{
    ui->setupUi(this);

    // Setup view.
    const FieldDefinitionList& fieldDefinitions = this->fieldDefinitionsController.getFieldDefinitions();

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

    // Add all fields.
    this->updateTable();

    this->ui->tableWidget->resizeColumnsToContents();
    this->ui->tableWidget->horizontalHeader()->setStretchLastSection(true);

    // Enable sorting.
    this->ui->tableWidget->setSortingEnabled(true);

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

void FieldDefinitionsWindow::showEvent(QShowEvent* event)
{
    Q_UNUSED(event);

    // Fields might have changed since last show (e.g. component removed).
    this->updateTable();
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
            const QString& fieldId = this->fieldDefinitionWindow->getFieldId();
            const QString& component = this->fieldDefinitionWindow->getFieldComponent();

            // Update model.
            FieldDefinition fieldDefinition =
                    this->fieldDefinitionsController.addFieldDefinition(
                        fieldId,
                        this->fieldDefinitionWindow->getFieldDisplayName(),
                        this->fieldDefinitionWindow->getFieldType(),
                        this->fieldDefinitionWindow->getDefaultValue(),
                        component,
                        this->fieldDefinitionWindow->getFieldDescription());

            this->recordsController.moveFieldToComponent(fieldId, QString(), component);

            // Update view.
            this->ui->tableWidget->insertRow(0);
            this->updateRow(0, fieldDefinition);
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
    QString fieldId = getSelectedFieldId();

    if (fieldId.isEmpty())
    {
        return;
    }

    // Get selected field definition.
    const FieldDefinition& fieldDefinition =
            this->fieldDefinitionsController.getFieldDefinition(fieldId);

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

        // Notify listeners.
        emit fieldChanged();
    }
}

void FieldDefinitionsWindow::on_actionDelete_Field_triggered()
{
    QString fieldId = this->getSelectedFieldId();

    if (fieldId.isEmpty())
    {
        return;
    }

    const int index = this->getFieldRow(fieldId);

    // Update model.
    this->fieldDefinitionsController.removeFieldDefinition(fieldId);
    this->recordsController.removeRecordField(fieldId);

    // Update view.
    this->ui->tableWidget->removeRow(index);

    // Notify listeners.
    emit fieldChanged();
}

void FieldDefinitionsWindow::on_actionFind_Usages_triggered()
{
    // Find usages.
    const QString& fieldId = this->getSelectedFieldId();
    this->findUsagesController.findUsagesOfField(fieldId);
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

int FieldDefinitionsWindow::getFieldRow(const QString& fieldId) const
{
    for (int i = 0; i < this->ui->tableWidget->rowCount(); ++i)
    {
        if (this->ui->tableWidget->item(i, 0)->data(Qt::DisplayRole) == fieldId)
        {
            return i;
        }
    }

    return -1;
}

QString FieldDefinitionsWindow::getSelectedFieldId() const
{
    QList<QTableWidgetItem*> selectedItems = this->ui->tableWidget->selectedItems();
    if (selectedItems.isEmpty())
    {
        return QString();
    }

    return selectedItems[0]->data(Qt::DisplayRole).toString();
}

void FieldDefinitionsWindow::updateTable()
{
    const FieldDefinitionList& fieldDefinitions = this->fieldDefinitionsController.getFieldDefinitions();

    this->ui->tableWidget->setSortingEnabled(false);

    for (int i = 0; i < fieldDefinitions.size(); ++i)
    {
        this->updateRow(i, fieldDefinitions[i]);
    }

    this->ui->tableWidget->setSortingEnabled(true);
}

void FieldDefinitionsWindow::updateMenus()
{
    bool hasSelection = !this->getSelectedFieldId().isEmpty();

    this->ui->actionEdit_Field->setEnabled(hasSelection);
    this->ui->actionDelete_Field->setEnabled(hasSelection);
    this->ui->actionFind_Usages->setEnabled(hasSelection);
}

void FieldDefinitionsWindow::updateFieldDefinition(const QString oldId, const QString newId, const QString& displayName, const QString& fieldType, const QVariant& defaultValue, const QString& description, const Component& component)
{
    const FieldDefinition& fieldDefinition = this->fieldDefinitionsController.getFieldDefinition(oldId);

    try
    {
        // Update model.
        const QString oldComponent = fieldDefinition.component;

        this->fieldDefinitionsController.updateFieldDefinition(oldId, newId, displayName, fieldType, defaultValue, component, description);
        this->recordsController.renameRecordField(oldId, newId);
        this->recordsController.moveFieldToComponent(newId, oldComponent, component);

        // Update view.
        const int i = this->getFieldRow(oldId);
        this->updateRow(i, fieldDefinition);
    }
    catch (std::out_of_range& e)
    {
        QMessageBox::critical(
                    this,
                    tr("Unable to edit field"),
                    e.what(),
                    QMessageBox::Close,
                    QMessageBox::Close);

        this->on_actionNew_Field_triggered();
    }
}

void FieldDefinitionsWindow::updateRow(const int i, const FieldDefinition& fieldDefinition)
{
    // Convert default value to string.
    QString defaultValueString = this->typesController.valueToString(fieldDefinition.defaultValue, fieldDefinition.fieldType);

    // Disable sorting before upading data (see http://doc.qt.io/qt-5.7/qtablewidget.html#setItem)
    this->ui->tableWidget->setSortingEnabled(false);

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

    // Enable sorting again.
    this->ui->tableWidget->setSortingEnabled(true);
}
