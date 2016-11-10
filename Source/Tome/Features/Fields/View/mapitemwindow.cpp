#include "mapitemwindow.h"
#include "ui_mapitemwindow.h"

#include <QPushButton>
#include <QMessageBox>

#include "fieldvaluewidget.h"
#include "../../Records/Controller/recordscontroller.h"
#include "../../Types/Controller/typescontroller.h"


using namespace Tome;


MapItemWindow::MapItemWindow(Tome::FacetsController& facetsController, Tome::RecordsController& recordsController, Tome::TypesController& typesController, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MapItemWindow),
    facetsController(facetsController),
    recordsController(recordsController),
    typesController(typesController)
{
    ui->setupUi(this);

    // Add widgets for specifying key and value.
    QFormLayout* layout = static_cast<QFormLayout*>(this->layout());

    this->keyWidget = new FieldValueWidget(this->facetsController, this->recordsController, this->typesController, this);
    layout->insertRow(0, tr("Key:"), this->keyWidget);

    this->valueWidget = new FieldValueWidget(this->facetsController, this->recordsController, this->typesController, this);
    layout->insertRow(1, tr("Value:"), this->valueWidget);
}

MapItemWindow::~MapItemWindow()
{
    delete ui;

    delete this->keyWidget;
    delete this->valueWidget;
}

QVariant MapItemWindow::getKey() const
{
    return this->keyWidget->getFieldValue();
}

QVariant MapItemWindow::getValue() const
{
    return this->valueWidget->getFieldValue();
}

void MapItemWindow::setKey(const QVariant& key)
{
    this->keyWidget->setFieldValue(key);
}

void MapItemWindow::setKeyType(const QString& keyType) const
{
    this->keyWidget->setFieldType(keyType);
}

void MapItemWindow::setValue(const QVariant& value)
{
    this->valueWidget->setFieldValue(value);
}

void MapItemWindow::setValueType(const QString& valueType) const
{
    this->valueWidget->setFieldType(valueType);
}

void MapItemWindow::accept()
{
    // Validate data.
    if (this->validate())
    {
        this->done(Accepted);
    }
}

void MapItemWindow::showEvent(QShowEvent* event)
{
    QDialog::showEvent(event);

    // Set OK as default button (instead of Cancel).
    QPushButton* okButton = this->ui->buttonBox->button(QDialogButtonBox::Ok);
    okButton->setAutoDefault(true);
    okButton->setDefault(true);
}

bool MapItemWindow::validate()
{
    // Key must be valid.
    QString validationError = this->keyWidget->validate();

    if (!validationError.isEmpty())
    {
        QMessageBox::information(
                    this,
                    tr("Invalid key"),
                    validationError,
                    QMessageBox::Close,
                    QMessageBox::Close);
        return false;
    }

    // Value must be valid.
    validationError = this->valueWidget->validate();

    if (!validationError.isEmpty())
    {
        QMessageBox::information(
                    this,
                    tr("Invalid value"),
                    validationError,
                    QMessageBox::Close,
                    QMessageBox::Close);
        return false;
    }

    return true;
}
