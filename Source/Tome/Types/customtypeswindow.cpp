#include "customtypeswindow.h"
#include "ui_customtypeswindow.h"

using namespace Tome;


CustomTypesWindow::CustomTypesWindow(QSharedPointer<Tome::Project> project, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CustomTypesWindow),
    enumerationWindow(0),
    listWindow(0),
    project(project)
{
    ui->setupUi(this);

    CustomTypesItemModel* model = new CustomTypesItemModel(project);
    this->viewModel = QSharedPointer<CustomTypesItemModel>(model);

    this->ui->listView->setModel(model);
}

CustomTypesWindow::~CustomTypesWindow()
{
    delete ui;
}

void CustomTypesWindow::on_actionNew_Custom_Type_triggered()
{
    // Show window.
    if (!this->enumerationWindow)
    {
        this->enumerationWindow = new EnumerationWindow(this);
    }

    int result = this->enumerationWindow->exec();

    if (result == QDialog::Accepted)
    {
        // Add new type.
        this->viewModel->addEnumeration(
                    this->enumerationWindow->getEnumerationName(),
                    this->enumerationWindow->getEnumerationMembers());
    }
}

void CustomTypesWindow::on_actionNew_List_triggered()
{
    // Show window.
    if (!this->listWindow)
    {
        this->listWindow = new ListWindow(this);
    }

    // Update type selection.
    this->listWindow->setProject(this->project);

    int result = this->listWindow->exec();

    if (result == QDialog::Accepted)
    {
        // Add new type.
        this->viewModel->addList(
                    this->listWindow->getListName(),
                    this->listWindow->getListItemType());
    }
}

void CustomTypesWindow::on_actionEdit_Custom_Type_triggered()
{
    // Get selected type.
    QModelIndexList selectedIndexes = this->ui->listView->selectionModel()->selectedRows();

    if (selectedIndexes.isEmpty())
    {
        return;
    }

    int index = selectedIndexes.first().row();
    QSharedPointer<CustomType> type = this->project->types[index];

    // Show window.
    if (!this->enumerationWindow)
    {
        this->enumerationWindow = new EnumerationWindow(this);
    }

    // Update view.
    this->enumerationWindow->setEnumerationName(type->name);
    this->enumerationWindow->setEnumerationMembers(type->getEnumeration());

    int result = this->enumerationWindow->exec();

    if (result == QDialog::Accepted)
    {
        // Update type.
        this->viewModel->updateEnumeration(
                    index,
                    this->enumerationWindow->getEnumerationName(),
                    this->enumerationWindow->getEnumerationMembers());
    }
}

void CustomTypesWindow::on_actionDelete_Custom_Type_triggered()
{
    // Get selected type.
    QModelIndexList selectedIndexes = this->ui->listView->selectionModel()->selectedRows();

    if (selectedIndexes.isEmpty())
    {
        return;
    }

    int index = selectedIndexes.first().row();

    // Delete type.
    this->viewModel->removeCustomType(index);
}

void CustomTypesWindow::on_listView_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index);
    this->on_actionEdit_Custom_Type_triggered();
}
