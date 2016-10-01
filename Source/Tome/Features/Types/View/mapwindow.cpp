#include "mapwindow.h"
#include "ui_mapwindow.h"

#include <QMessageBox>
#include "../Controller/typescontroller.h"

MapWindow::MapWindow(Tome::TypesController& typesController, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MapWindow),
    typesController(typesController)
{
    ui->setupUi(this);
}

MapWindow::~MapWindow()
{
    delete this->ui;
}

QString MapWindow::getMapName() const
{
    return this->ui->lineEditName->text();
}

QString MapWindow::getMapKeyType() const
{
    return this->ui->comboBoxKeyType->currentText();
}

QString MapWindow::getMapValueType() const
{
    return this->ui->comboBoxValueType->currentText();
}

QString MapWindow::getTypeSetName() const
{
    return this->ui->comboBoxTypeSet->currentText();
}

void MapWindow::init()
{
    // Set type names.
    this->ui->comboBoxKeyType->clear();
    this->ui->comboBoxValueType->clear();

    const QStringList& typeNames = this->typesController.getTypeNames();

    for (int i = 0; i < typeNames.length(); ++i)
    {
        this->ui->comboBoxKeyType->addItem(typeNames[i]);
        this->ui->comboBoxValueType->addItem(typeNames[i]);
    }
}

void MapWindow::setMapName(const QString& mapName)
{
    this->ui->lineEditName->setText(mapName);
}

void MapWindow::setMapKeyType(const QString& keyType)
{
    this->ui->comboBoxKeyType->setCurrentText(keyType);
}

void MapWindow::setMapValueType(const QString& valueType)
{
    this->ui->comboBoxValueType->setCurrentText(valueType);
}

void MapWindow::setTypeSetName(const QString& typeSetName)
{
    this->ui->comboBoxTypeSet->setCurrentText(typeSetName);
}

void MapWindow::setTypeSetNames(const QStringList& typeSetNames)
{
    this->ui->comboBoxTypeSet->clear();
    this->ui->comboBoxTypeSet->addItems(typeSetNames);
}

void MapWindow::accept()
{
    // Validate data.
    if (this->validate())
    {
        this->done(Accepted);
    }
}

bool MapWindow::validate()
{
    // Name must not be empty.
    if (this->getMapName().isEmpty())
    {
        QMessageBox::information(
                    this,
                    tr("Missing data"),
                    tr("Please specify a name for the map."),
                    QMessageBox::Close,
                    QMessageBox::Close);
        return false;
    }

    // Key type must not be empty.
    if (this->getMapKeyType().isEmpty())
    {
        QMessageBox::information(
                    this,
                    tr("Missing data"),
                    tr("Please specify a type for the keys of the map."),
                    QMessageBox::Close,
                    QMessageBox::Close);
        return false;
    }

    // Value type must not be empty.
    if (this->getMapValueType().isEmpty())
    {
        QMessageBox::information(
                    this,
                    tr("Missing data"),
                    tr("Please specify a type for the values of the map."),
                    QMessageBox::Close,
                    QMessageBox::Close);
        return false;
    }

    return true;
}

