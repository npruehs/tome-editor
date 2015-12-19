#include "fielddefinitionswindow.h"
#include "ui_fielddefinitionswindow.h"

#include "fielddefinitionstablemodel.h"

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
}

FieldDefinitionsWindow::~FieldDefinitionsWindow()
{
    delete ui;
}

void FieldDefinitionsWindow::on_actionNew_Field_triggered()
{
    // Show window.
    if (!this->fieldDefinitionWindow)
    {
        this->fieldDefinitionWindow = new FieldDefinitionWindow(this);
    }

    int result = this->fieldDefinitionWindow->exec();

    if (result == QDialog::Accepted)
    {
        // Add new field.
        this->viewModel->addFieldDefinition
                (this->fieldDefinitionWindow->getFieldId(),
                 this->fieldDefinitionWindow->getFieldDisplayName(),
                 this->fieldDefinitionWindow->getFieldType(),
                 this->fieldDefinitionWindow->getFieldDescription());
    }
}

void FieldDefinitionsWindow::on_actionEdit_Field_triggered()
{
    int index = selectedFieldIndex();

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

    // Update view.
    this->fieldDefinitionWindow->setFieldId(fieldDefinition->id);
    this->fieldDefinitionWindow->setFieldDisplayName(fieldDefinition->displayName);
    this->fieldDefinitionWindow->setFieldType(fieldDefinition->fieldType);
    this->fieldDefinitionWindow->setFieldDescription(fieldDefinition->description);

    int result = this->fieldDefinitionWindow->exec();

    if (result == QDialog::Accepted)
    {
        // Update field.
        this->viewModel->updateFieldDefinition
                (index,
                 this->fieldDefinitionWindow->getFieldId(),
                 this->fieldDefinitionWindow->getFieldDisplayName(),
                 this->fieldDefinitionWindow->getFieldType(),
                 this->fieldDefinitionWindow->getFieldDescription());
    }
}

void FieldDefinitionsWindow::on_actionDelete_Field_triggered()
{
    int index = selectedFieldIndex();

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

int FieldDefinitionsWindow::selectedFieldIndex() const
{
    QModelIndexList selectedIndices = this->ui->tableView->selectionModel()->selectedRows();
    return selectedIndices.count() > 0 ? selectedIndices.first().row() : -1;
}

