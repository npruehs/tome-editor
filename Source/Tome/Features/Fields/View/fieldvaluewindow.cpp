#include "fieldvaluewindow.h"
#include "ui_fieldvaluewindow.h"

using namespace Tome;


FieldValueWindow::FieldValueWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FieldValueWindow)
{
    ui->setupUi(this);

    // Add widget for specifying the field value.
    this->fieldValueWidget = new FieldValueWidget(this);
    QFormLayout* layout = static_cast<QFormLayout*>(this->layout());
    layout->insertRow(2, tr("Value:"), this->fieldValueWidget);
}

FieldValueWindow::~FieldValueWindow()
{
    delete this->ui;

    delete this->fieldValueWidget;
}

QString FieldValueWindow::getFieldValue() const
{
    return this->fieldValueWidget->getFieldValue();
}

void FieldValueWindow::setFieldDescription(const QString& description)
{
    this->ui->labelDescriptionValue->setText(description);
}

void FieldValueWindow::setFieldDisplayName(const QString& displayName)
{
    this->ui->labelDisplayNameValue->setText(displayName);
}

void FieldValueWindow::setFieldValue(const QString& fieldValue)
{
    this->fieldValueWidget->setFieldValue(fieldValue);
}

void FieldValueWindow::setCustomFieldType(const CustomType& fieldType)
{
    this->ui->labelTypeValue->setText(fieldType.name);
    this->fieldValueWidget->setCustomFieldType(fieldType);
}

void FieldValueWindow::setFieldType(const QString& fieldType) const
{
    this->ui->labelTypeValue->setText(fieldType);
    this->fieldValueWidget->setFieldType(fieldType);
}

void FieldValueWindow::setEnumeration(const QStringList& recordNames)
{
    this->fieldValueWidget->setEnumeration(recordNames);
}

void FieldValueWindow::showEvent(QShowEvent* event)
{
    QDialog::showEvent(event);
    this->fieldValueWidget->setFocus();
}
