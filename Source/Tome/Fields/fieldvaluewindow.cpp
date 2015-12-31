#include "fieldvaluewindow.h"
#include "ui_fieldvaluewindow.h"

#include "../Values/valueconverter.h"

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
    delete ui;
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

void FieldValueWindow::setFieldType(const Tome::FieldType::FieldType& fieldType) const
{
    ValueConverter valueConverter;
    const QString fieldTypeName = valueConverter.FieldTypeToString(fieldType);
    this->ui->labelTypeValue->setText(fieldTypeName);
    this->fieldValueWidget->setFieldType(fieldType);
}

void FieldValueWindow::setRecordNames(const QStringList& recordNames)
{
    this->fieldValueWidget->setRecordNames(recordNames);
}

void FieldValueWindow::showEvent(QShowEvent* event)
{
    QDialog::showEvent(event);
    this->fieldValueWidget->setFocus();
}
