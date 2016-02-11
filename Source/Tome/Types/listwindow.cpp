#include "listwindow.h"
#include "ui_listwindow.h"

#include <QMessageBox>


ListWindow::ListWindow(Tome::TypesController& typesController, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ListWindow),
    typesController(typesController)
{
    ui->setupUi(this);
}

ListWindow::~ListWindow()
{
    delete ui;
}

QString ListWindow::getListName() const
{
    return this->ui->lineEdit->text();
}

QString ListWindow::getListItemType() const
{
    return this->ui->comboBox->currentText();
}

void ListWindow::setListName(const QString& listName)
{
    this->ui->lineEdit->setText(listName);
}

void ListWindow::setListItemType(const QString& itemType)
{
    this->ui->comboBox->setCurrentText(itemType);
}

void ListWindow::accept()
{
    // Validate data.
    if (this->validate())
    {
        this->done(Accepted);
    }
}

int ListWindow::exec()
{
    // Set type names.
    this->ui->comboBox->clear();

    const QStringList& typeNames = this->typesController.getTypeNames();

    for (int i = 0; i < typeNames.length(); ++i)
    {
        this->ui->comboBox->addItem(typeNames[i]);
    }

    // Show window.
    return QDialog::exec();
}

bool ListWindow::validate()
{
    // Name must not be empty.
    if (this->getListName().isEmpty())
    {
        QMessageBox::information(
                    this,
                    tr("Missing data"),
                    tr("Please specify a name for the list."),
                    QMessageBox::Close,
                    QMessageBox::Close);
        return false;
    }

    // Item type must not be empty.
    if (this->getListItemType().isEmpty())
    {
        QMessageBox::information(
                    this,
                    tr("Missing data"),
                    tr("Please specify a type for the items of the list."),
                    QMessageBox::Close,
                    QMessageBox::Close);
        return false;
    }

    return true;
}
