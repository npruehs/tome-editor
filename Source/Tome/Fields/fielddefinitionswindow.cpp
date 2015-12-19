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
