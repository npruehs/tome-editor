#include "listwindow.h"
#include "ui_listwindow.h"

#include <QMessageBox>
#include "../Controller/typescontroller.h"


ListWindow::ListWindow(Tome::TypesController& typesController, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ListWindow),
    typesController(typesController)
{
    ui->setupUi(this);
}

ListWindow::~ListWindow()
{
    delete this->ui;
}

QString ListWindow::getListName() const
{
    return this->ui->lineEdit->text();
}

QString ListWindow::getListItemType() const
{
    return this->ui->comboBoxItemType->currentText();
}

QString ListWindow::getTypeSetName() const
{
    return this->ui->comboBoxTypeSet->currentText();
}

void ListWindow::init()
{
    // Set type names.
    this->ui->comboBoxItemType->clear();

    const QStringList& typeNames = this->typesController.getTypeNames();

    for (int i = 0; i < typeNames.length(); ++i)
    {
        this->ui->comboBoxItemType->addItem(typeNames[i]);
    }

    // Set type set names.
    const QStringList typeSetNames = this->typesController.getCustomTypeSetNames();
    this->setTypeSetNames(typeSetNames);
    this->setTypeSetName(typeSetNames.first());
}

void ListWindow::setListName(const QString& listName)
{
    this->ui->lineEdit->setText(listName);
}

void ListWindow::setListItemType(const QString& itemType)
{
    this->ui->comboBoxItemType->setCurrentText(itemType);
}

void ListWindow::setTypeSetName(const QString& typeSet)
{
    this->ui->comboBoxTypeSet->setCurrentText(typeSet);
}

void ListWindow::setTypeSetNames(const QStringList& typeSets)
{
    this->ui->comboBoxTypeSet->clear();
    this->ui->comboBoxTypeSet->addItems(typeSets);
}

void ListWindow::accept()
{
    // Validate data.
    if (this->validate())
    {
        this->done(Accepted);
    }
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
