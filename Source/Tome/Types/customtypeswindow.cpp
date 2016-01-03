#include "customtypeswindow.h"
#include "ui_customtypeswindow.h"

using namespace Tome;


CustomTypesWindow::CustomTypesWindow(QSharedPointer<Tome::Project> project, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CustomTypesWindow),
    customTypeWindow(0),
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
    if (!this->customTypeWindow)
    {
        this->customTypeWindow = new CustomTypeWindow(this);
    }

    int result = this->customTypeWindow->exec();

    if (result == QDialog::Accepted)
    {
         // Add new type.
        this->viewModel->addCustomType(
                    this->customTypeWindow->getCustomTypeName(),
                    this->customTypeWindow->getCustomTypeEnumeration());
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
    if (!this->customTypeWindow)
    {
        this->customTypeWindow = new CustomTypeWindow(this);
    }

    // Update view.
    this->customTypeWindow->setCustomTypeName(type->name);
    this->customTypeWindow->setCustomTypeEnumeration(type->getEnumeration());

    int result = this->customTypeWindow->exec();

    if (result == QDialog::Accepted)
    {
        // Update type.
        this->viewModel->updateCustomType(
                    index,
                    this->customTypeWindow->getCustomTypeName(),
                    this->customTypeWindow->getCustomTypeEnumeration());
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
