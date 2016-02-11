#include "listitemwindow.h"
#include "ui_listitemwindow.h"

#include "fieldvaluewidget.h"


using namespace Tome;


ListItemWindow::ListItemWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ListItemWindow)
{
    ui->setupUi(this);

    // Add widget for specifying the list item value.
    this->fieldValueWidget = new FieldValueWidget(this);
    QFormLayout* layout = static_cast<QFormLayout*>(this->layout());
    layout->insertRow(0, tr("Value:"), this->fieldValueWidget);
}

ListItemWindow::~ListItemWindow()
{
    delete ui;
}

QString ListItemWindow::getValue() const
{
    return this->fieldValueWidget->getFieldValue();
}

void ListItemWindow::setValue(const QString& value)
{
    this->fieldValueWidget->setFieldValue(value);
}

void ListItemWindow::setCustomFieldType(const CustomType& fieldType)
{
    this->fieldValueWidget->setCustomFieldType(fieldType);
}

void ListItemWindow::setFieldType(const QString& fieldType) const
{
    this->fieldValueWidget->setFieldType(fieldType);
}

void ListItemWindow::setEnumeration(const QStringList& recordNames)
{
    this->fieldValueWidget->setEnumeration(recordNames);
}
