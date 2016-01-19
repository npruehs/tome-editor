#include "fielddefinitionswindow.h"
#include "ui_fielddefinitionswindow.h"

#include "fielddefinitionstablemodel.h"
#include "../Types/builtintype.h"

using namespace Tome;


FieldDefinitionsWindow::FieldDefinitionsWindow(QSharedPointer<Tome::Project> project, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FieldDefinitionsWindow),
    fieldDefinitionWindow(0),
    project(project)
{
    ui->setupUi(this);

    FieldDefinitionsTableModel* model = new FieldDefinitionsTableModel(this, project);
    this->viewModel = QSharedPointer<FieldDefinitionsTableModel>(model);

    this->ui->tableView->setModel(model);

    // Listen for selection changes.
    connect(
                this->ui->tableView->selectionModel(),
                SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
                SLOT(tableViewSelectionChanged(const QItemSelection &, const QItemSelection &))
                );

    this->updateMenus();
}

FieldDefinitionsWindow::~FieldDefinitionsWindow()
{
    delete ui;
}

void FieldDefinitionsWindow::on_actionNew_Field_triggered()
{
    // Setup window.
    if (!this->fieldDefinitionWindow)
    {
        this->fieldDefinitionWindow = new FieldDefinitionWindow(this);
    }

    // Update project reference.
    this->fieldDefinitionWindow->setProject(this->project);

    // Show window.
    int result = this->fieldDefinitionWindow->exec();

    if (result == QDialog::Accepted)
    {
        // Add new field.
        this->viewModel->addFieldDefinition
                (this->fieldDefinitionWindow->getFieldId(),
                 this->fieldDefinitionWindow->getFieldDisplayName(),
                 this->fieldDefinitionWindow->getFieldType(),
                 this->fieldDefinitionWindow->getDefaultValue(),
                 this->fieldDefinitionWindow->getFieldDescription(),
                 this->fieldDefinitionWindow->getFieldComponent());
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
    QSharedPointer<FieldDefinition> fieldDefinition =
            this->project->fieldDefinitionSets[0]->fieldDefinitions[index];

    // Show window.
    if (!this->fieldDefinitionWindow)
    {
        this->fieldDefinitionWindow = new FieldDefinitionWindow(this);
    }

    // Update project reference.
    this->fieldDefinitionWindow->setProject(this->project);

    // Update view.
    this->fieldDefinitionWindow->setFieldId(fieldDefinition->id);
    this->fieldDefinitionWindow->setFieldDisplayName(fieldDefinition->displayName);
    this->fieldDefinitionWindow->setFieldType(fieldDefinition->fieldType);
    this->fieldDefinitionWindow->setDefaultValue(fieldDefinition->defaultValue);
    this->fieldDefinitionWindow->setFieldDescription(fieldDefinition->description);
    this->fieldDefinitionWindow->setFieldComponent(fieldDefinition->component);

    int result = this->fieldDefinitionWindow->exec();

    if (result == QDialog::Accepted)
    {
        // Update field.
        this->viewModel->updateFieldDefinition
                (index,
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

    this->viewModel->removeFieldDefinition(index);
}

void FieldDefinitionsWindow::on_tableView_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index);
    this->on_actionEdit_Field_triggered();
}

void FieldDefinitionsWindow::tableViewSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected)
{
    Q_UNUSED(selected);
    Q_UNUSED(deselected);
    this->updateMenus();
}

int FieldDefinitionsWindow::getSelectedFieldIndex() const
{
    QModelIndexList selectedIndexes = this->ui->tableView->selectionModel()->selectedRows();
    return selectedIndexes.count() > 0 ? selectedIndexes.first().row() : -1;
}

void FieldDefinitionsWindow::updateMenus()
{
    bool hasSelection = getSelectedFieldIndex() >= 0;

    this->ui->actionEdit_Field->setEnabled(hasSelection);
    this->ui->actionDelete_Field->setEnabled(hasSelection);
}
