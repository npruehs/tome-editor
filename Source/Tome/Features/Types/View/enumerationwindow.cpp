#include "enumerationwindow.h"
#include "ui_enumerationwindow.h"

#include <QStringListModel>

#include "../../../Util/listutils.h"


using namespace Tome;


EnumerationWindow::EnumerationWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EnumerationWindow),
    enumerationMemberWindow(0)
{
    ui->setupUi(this);
}

EnumerationWindow::~EnumerationWindow()
{
    delete ui;
}

QString EnumerationWindow::getEnumerationName() const
{
    return this->ui->lineEdit->text();
}

QStringList EnumerationWindow::getEnumerationMembers() const
{
    return this->enumeration;
}

void EnumerationWindow::setEnumerationName(const QString& typeName)
{
    this->ui->lineEdit->setText(typeName);
}

void EnumerationWindow::setEnumerationMembers(const QStringList enumeration)
{
    // Update model.
    this->enumeration = enumeration;

    // Update view.
    this->ui->listWidget->clear();
    this->ui->listWidget->insertItems(0, enumeration);
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
        QString item = this->enumerationMemberWindow->getText();
        int index = findInsertionIndex(this->enumeration, item);

        // Update model.
        this->enumeration.insert(index, item);

        // Update view.
        this->ui->listWidget->insertItem(index, item);
    }
}

void EnumerationWindow::on_actionDelete_Member_triggered()
{
    QModelIndexList selectedIndexes = this->ui->listWidget->selectionModel()->selectedRows();

    if (selectedIndexes.isEmpty())
    {
        return;
    }

    int row = selectedIndexes.first().row();

    // Update model.
    this->enumeration.removeAt(row);

    // Update view.
    this->ui->listWidget->takeItem(row);
}
