#include "fielddefinitionswindow.h"
#include "ui_fielddefinitionswindow.h"

#include "fielddefinitionstablemodel.h"

using namespace Tome;


FieldDefinitionsWindow::FieldDefinitionsWindow(QSharedPointer<Tome::Project> project, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FieldDefinitionsWindow),
    fieldDefinitionWindow(0),
    project(project),
    viewModel(QSharedPointer<FieldDefinitionsTableModel>::create(project))
{
    ui->setupUi(this);

    this->ui->tableView->setModel(this->viewModel.data());
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
        QSharedPointer<Tome::FieldDefinition> fieldDefinition = QSharedPointer<Tome::FieldDefinition>::create();
        fieldDefinition->id = this->fieldDefinitionWindow->getFieldId();
        fieldDefinition->displayName = this->fieldDefinitionWindow->getFieldDisplayName();
        fieldDefinition->fieldType = this->fieldDefinitionWindow->getFieldType();
        fieldDefinition->description = this->fieldDefinitionWindow->getFieldDescription();

        this->project->fieldDefinitionSets[0]->fieldDefinitions.push_back(fieldDefinition);
    }
}
