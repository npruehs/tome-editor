#include "fielddefinitionswindow.h"
#include "ui_fielddefinitionswindow.h"

#include "fielddefinitionstablemodel.h"

using namespace Tome;


FieldDefinitionsWindow::FieldDefinitionsWindow(QSharedPointer<Tome::Project> project, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FieldDefinitionsWindow),
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
