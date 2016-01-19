#include "customtypewindow.h"
#include "ui_customtypewindow.h"

#include <QStringListModel>

using namespace Tome;


CustomTypeWindow::CustomTypeWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CustomTypeWindow),
    customTypeMemberWindow(0)
{
    ui->setupUi(this);

    QStringListModel* model = new QStringListModel();
    model->setStringList(QStringList());
    this->ui->listView->setModel(model);
}

CustomTypeWindow::~CustomTypeWindow()
{
    delete ui;
}

QString CustomTypeWindow::getCustomTypeName() const
{
    return this->ui->lineEdit->text();
}

QStringList CustomTypeWindow::getCustomTypeEnumeration() const
{
    QStringListModel* model = static_cast<QStringListModel*>(this->ui->listView->model());
    return model->stringList();
}

void CustomTypeWindow::setCustomTypeName(const QString& typeName)
{
    this->ui->lineEdit->setText(typeName);
}

void CustomTypeWindow::setCustomTypeEnumeration(const QStringList& enumeration)
{
    QStringListModel* model = static_cast<QStringListModel*>(this->ui->listView->model());
    model->setStringList(enumeration);
}

void CustomTypeWindow::on_actionNew_Member_triggered()
{
    // Show window.
    if (!this->customTypeMemberWindow)
    {
        this->customTypeMemberWindow = new CustomTypeMemberWindow(this);
    }

    int result = this->customTypeMemberWindow->exec();

    if (result == QDialog::Accepted)
    {
        // Add new type member.
        QStringListModel* model = static_cast<QStringListModel*>(this->ui->listView->model());
        QStringList stringList = model->stringList();
        stringList << this->customTypeMemberWindow->getText();
        stringList.sort();
        model->setStringList(stringList);
    }
}

void CustomTypeWindow::on_actionDelete_Member_triggered()
{
    QModelIndexList selectedIndexes = this->ui->listView->selectionModel()->selectedRows();

    if (selectedIndexes.isEmpty())
    {
        return;
    }

    int row = selectedIndexes.first().row();

    QStringListModel* model = static_cast<QStringListModel*>(this->ui->listView->model());
    QStringList stringList = model->stringList();
    stringList.removeAt(row);
    model->setStringList(stringList);
}
