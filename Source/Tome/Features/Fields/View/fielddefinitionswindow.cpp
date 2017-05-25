#include "fielddefinitionswindow.h"
#include "ui_fielddefinitionswindow.h"

#include <stdexcept>

#include <QMessageBox>

#include "fielddefinitionwindow.h"
#include "../Controller/fielddefinitionscontroller.h"
#include "../Controller/Commands/addfielddefinitioncommand.h"
#include "../Controller/Commands/removefielddefinitioncommand.h"
#include "../Controller/Commands/updatefielddefinitioncommand.h"
#include "../Model/fielddefinition.h"
#include "../../Facets/Controller/facetscontroller.h"
#include "../../Components/Controller/componentscontroller.h"
#include "../../Records/Controller/recordscontroller.h"
#include "../../Search/Controller/findusagescontroller.h"
#include "../../Types/Controller/typescontroller.h"
#include "../../Types/Model/builtintype.h"
#include "../../Undo/Controller/undocontroller.h"
#include "../../../Util/listutils.h"

using namespace Tome;


FieldDefinitionsWindow::FieldDefinitionsWindow(FieldDefinitionsController& fieldDefinitionsController,
        ComponentsController& componentsController,
        FacetsController& facetsController,
        FindUsagesController& findUsagesController,
        ProjectController& projectController,
        RecordsController& recordsController,
        TypesController& typesController,
        UndoController& undoController,
        QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FieldDefinitionsWindow),
    fieldDefinitionsController(fieldDefinitionsController),
    componentsController(componentsController),
    facetsController(facetsController),
    findUsagesController(findUsagesController),
    projectController(projectController),
    recordsController(recordsController),
    typesController(typesController),
    undoController(undoController),
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

    // Connect signals.
    connect(
                &this->fieldDefinitionsController,
                SIGNAL(fieldDefinitionAdded(const Tome::FieldDefinition&)),
                SLOT(onFieldDefinitionAdded(const Tome::FieldDefinition&)));

    connect(
                &this->fieldDefinitionsController,
                SIGNAL(fieldDefinitionRemoved(const Tome::FieldDefinition&)),
                SLOT(onFieldDefinitionRemoved(const Tome::FieldDefinition&)));

    connect(
                &this->fieldDefinitionsController,
                SIGNAL(fieldDefinitionUpdated(const Tome::FieldDefinition&, const Tome::FieldDefinition&)),
                SLOT(onFieldDefinitionUpdated(const Tome::FieldDefinition&, const Tome::FieldDefinition&)));

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
    Q_UNUSED(event)

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
                    this->facetsController,
                    this->projectController,
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
            const QString& fieldDisplayName = this->fieldDefinitionWindow->getFieldDisplayName();
            const QString& fieldType = this->fieldDefinitionWindow->getFieldType();
            const QVariant& fieldDefaultValue = this->fieldDefinitionWindow->getDefaultValue();
            const QString& fieldComponent = this->fieldDefinitionWindow->getFieldComponent();
            const QString& fieldDescription = this->fieldDefinitionWindow->getFieldDescription();
            const QString& fieldDefinitionSetName = this->fieldDefinitionWindow->getFieldDefinitionSetName();

            // Update model.
            AddFieldDefinitionCommand* command = new AddFieldDefinitionCommand(
                        this->fieldDefinitionsController,
                        fieldId,
                        fieldDisplayName,
                        fieldType,
                        fieldDefaultValue,
                        fieldComponent,
                        fieldDescription,
                        fieldDefinitionSetName);
            this->undoController.doCommand(command);
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
                    this->facetsController,
                    this->projectController,
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
    this->fieldDefinitionWindow->setFieldDefinitionSetNames(this->fieldDefinitionsController.getFieldDefinitionSetNames());
    this->fieldDefinitionWindow->setFieldDefinitionSetName(fieldDefinition.fieldDefinitionSetName);

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
                 this->fieldDefinitionWindow->getFieldComponent(),
                 this->fieldDefinitionWindow->getFieldDefinitionSetName());

        // Notify listeners.
        emit fieldChanged(this->fieldDefinitionWindow->getFieldId());
    }
}

void FieldDefinitionsWindow::on_actionDelete_Field_triggered()
{
    QString fieldId = this->getSelectedFieldId();

    if (fieldId.isEmpty())
    {
        return;
    }

    // Update model.
    RemoveFieldDefinitionCommand* command = new RemoveFieldDefinitionCommand(
                this->fieldDefinitionsController,
                this->recordsController,
                fieldId);
    this->undoController.doCommand(command);
}

void FieldDefinitionsWindow::on_actionFind_Usages_triggered()
{
    // Find usages.
    const QString& fieldId = this->getSelectedFieldId();
    this->findUsagesController.findUsagesOfField(fieldId);
}

void FieldDefinitionsWindow::on_tableWidget_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index)
    this->on_actionEdit_Field_triggered();
}

void FieldDefinitionsWindow::onFieldDefinitionAdded(const FieldDefinition& fieldDefinition)
{
    // Update view.
    this->ui->tableWidget->insertRow(0);
    this->updateRow(0, fieldDefinition, true);
}

void FieldDefinitionsWindow::onFieldDefinitionRemoved(const FieldDefinition& fieldDefinition)
{
    // Update view.
    const int index = this->getFieldRow(fieldDefinition.id);
    this->ui->tableWidget->removeRow(index);

    // Notify listeners.
    emit fieldChanged(fieldDefinition.id);
}

void FieldDefinitionsWindow::onFieldDefinitionUpdated(const FieldDefinition& oldFieldDefinition, const FieldDefinition& newFieldDefinition)
{
    Q_UNUSED(oldFieldDefinition)
    Q_UNUSED(newFieldDefinition)

    // Update view.
    this->updateTable();
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

    return selectedItems.first()->data(Qt::DisplayRole).toString();
}

void FieldDefinitionsWindow::updateTable()
{
    const FieldDefinitionList& fieldDefinitions = this->fieldDefinitionsController.getFieldDefinitions();

    this->ui->tableWidget->setSortingEnabled(false);

    if (fieldDefinitions.count() != this->ui->tableWidget->rowCount())
    {
        this->ui->tableWidget->setRowCount(fieldDefinitions.size());
    }

    for (int i = 0; i < fieldDefinitions.size(); ++i)
    {
        this->updateRow(i, fieldDefinitions[i], false);
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

void FieldDefinitionsWindow::updateFieldDefinition(const QString oldId,
        const QString newId,
        const QString& displayName,
        const QString& fieldType,
        const QVariant& defaultValue,
        const QString& description,
        const Component& component,
        const QString& fieldDefinitionSetName)
{
    const FieldDefinition& fieldDefinition = this->fieldDefinitionsController.getFieldDefinition(oldId);

    try
    {
        const QString oldComponent = fieldDefinition.component;

        // Update model.
        UpdateFieldDefinitionCommand* command = new UpdateFieldDefinitionCommand(
                    this->fieldDefinitionsController,
                    oldId,
                    newId,
                    displayName,
                    fieldType,
                    defaultValue,
                    component,
                    description,
                    fieldDefinitionSetName);
        this->undoController.doCommand(command);
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

void FieldDefinitionsWindow::updateRow(const int i, const FieldDefinition& fieldDefinition, bool disableSorting)
{
    // Convert default value to string.
    QString defaultValueString = this->typesController.valueToString(fieldDefinition.defaultValue, fieldDefinition.fieldType);

    // Disable sorting before upading data (see http://doc.qt.io/qt-5.7/qtablewidget.html#setItem)
    if (disableSorting)
    {
        this->ui->tableWidget->setSortingEnabled(false);
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

    // Enable sorting again.
    if (disableSorting)
    {
        this->ui->tableWidget->setSortingEnabled(true);
    }
}
