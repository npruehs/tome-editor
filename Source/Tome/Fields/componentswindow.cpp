#include "componentswindow.h"
#include "ui_componentswindow.h"

using namespace Tome;

ComponentsWindow::ComponentsWindow(QSharedPointer<Tome::Project> project, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ComponentsWindow),
    componentWindow(0),
    project(project)
{
    ui->setupUi(this);

    ComponentsItemModel* model = new ComponentsItemModel(project);
    this->viewModel = QSharedPointer<ComponentsItemModel>(model);

    this->ui->listView->setModel(model);
}

ComponentsWindow::~ComponentsWindow()
{
    delete ui;
}

void ComponentsWindow::on_actionNew_Component_triggered()
{
    // Show window.
    if (!this->componentWindow)
    {
        this->componentWindow = new ComponentWindow(this);
    }

    int result = this->componentWindow->exec();

    if (result == QDialog::Accepted)
    {
        // Add new component.
        this->viewModel->addComponent(this->componentWindow->getComponentName());
    }
}

void ComponentsWindow::on_actionDelete_Component_triggered()
{
    QModelIndexList selectedIndexes = this->ui->listView->selectionModel()->selectedRows();

    if (selectedIndexes.isEmpty())
    {
        return;
    }

    int row = selectedIndexes.first().row();
    this->viewModel->removeComponent(row);
}
