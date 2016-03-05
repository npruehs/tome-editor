#include "listitemwindow.h"
#include "ui_listitemwindow.h"

#include "fieldvaluewidget.h"
#include "../../Records/Controller/recordscontroller.h"
#include "../../Types/Controller/typescontroller.h"
#include "../../Types/Model/customtype.h"


using namespace Tome;


ListItemWindow::ListItemWindow(Tome::RecordsController& recordsController, Tome::TypesController& typesController, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ListItemWindow),
    recordsController(recordsController),
    typesController(typesController)
{
    ui->setupUi(this);

    // Add widget for specifying the list item value.
    this->fieldValueWidget = new FieldValueWidget(this->recordsController, this->typesController, this);
    QFormLayout* layout = static_cast<QFormLayout*>(this->layout());
    layout->insertRow(0, tr("Value:"), this->fieldValueWidget);
}

ListItemWindow::~ListItemWindow()
{
    delete this->ui;

    delete this->fieldValueWidget;
}

QVariant ListItemWindow::getValue() const
{
    return this->fieldValueWidget->getFieldValue();
}

void ListItemWindow::setValue(const QVariant& value)
{
    this->fieldValueWidget->setFieldValue(value);
}

void ListItemWindow::setFieldType(const QString& fieldType) const
{
    this->fieldValueWidget->setFieldType(fieldType);
}
