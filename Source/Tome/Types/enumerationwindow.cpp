#include "enumerationwindow.h"
#include "ui_enumerationwindow.h"

#include <QStringListModel>

using namespace Tome;


EnumerationWindow::EnumerationWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CustomTypeWindow),
    enumerationMemberWindow(0)
{
    ui->setupUi(this);

    QStringListModel* model = new QStringListModel();
    model->setStringList(QStringList());
    this->ui->listView->setModel(model);
}

EnumerationWindow::~EnumerationWindow()
{
    delete ui;
}

QString EnumerationWindow::getCustomTypeName() const
{
    return this->ui->lineEdit->text();
}

QStringList EnumerationWindow::getCustomTypeEnumeration() const
{
    QStringListModel* model = static_cast<QStringListModel*>(this->ui->listView->model());
    return model->stringList();
}

void EnumerationWindow::setCustomTypeName(const QString& typeName)
{
    this->ui->lineEdit->setText(typeName);
}

void EnumerationWindow::setCustomTypeEnumeration(const QStringList& enumeration)
{
    QStringListModel* model = static_cast<QStringListModel*>(this->ui->listView->model());
    model->setStringList(enumeration);
}

void EnumerationWindow::on_actionNew_Member_triggered()
{
    // Show window.
    if (!this->enumerationMemberWindow)
    {
        this->enumerationMemberWindow = new EnumerationMemberWindow(this);
    }

    int result = this->enumerationMemberWindow->exec();

    if (result == QDialog::Accepted)
    {
        // Add new type member.
        QStringListModel* model = static_cast<QStringListModel*>(this->ui->listView->model());
        QStringList stringList = model->stringList();
        stringList << this->enumerationMemberWindow->getText();
        stringList.sort();
        model->setStringList(stringList);
    }
}

void EnumerationWindow::on_actionDelete_Member_triggered()
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
