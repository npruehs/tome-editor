#include "fieldvaluewidget.h"

using namespace Tome;


FieldValueWidget::FieldValueWidget(QWidget *parent) :
    QWidget(parent),
    fieldType(FieldType::String)
{
    // Create layout.
    this->layout = new QVBoxLayout();

    // Add field value widgets.
    this->lineEdit = new QLineEdit();
    this->layout->addWidget(this->lineEdit);

    this->spinBox = new QSpinBox();
    this->spinBox->setMinimum(-2147483647);
    this->spinBox->setMaximum(2147483647);
    this->layout->addWidget(this->spinBox);

    this->setLayout(this->layout);
    this->layout->setContentsMargins(0, 0, 0, 0);
}

FieldValueWidget::~FieldValueWidget()
{
}

FieldType::FieldType FieldValueWidget::getFieldType() const
{
    return this->fieldType;
}

QString FieldValueWidget::getFieldValue() const
{
    switch (this->getFieldType())
    {
        case FieldType::Int:
            return this->spinBox->text();
            break;

        case FieldType::String:
            return this->lineEdit->text();
            break;
    }

    return QString();
}

void FieldValueWidget::setFieldType(const FieldType::FieldType& fieldType)
{
    // Set new field type.
    this->fieldType = fieldType;

    // Update view.
    switch (fieldType)
    {
        case FieldType::Int:
            this->lineEdit->hide();
            this->spinBox->show();
            break;

        case FieldType::String:
            this->lineEdit->show();
            this->spinBox->hide();
            break;

        default:
            this->lineEdit->hide();
            this->spinBox->hide();
            break;
    }
}

void FieldValueWidget::setFieldValue(const QString& fieldValue)
{
    switch (this->getFieldType())
    {
        case FieldType::Int:
            {
                int value = fieldValue.toInt();
                this->spinBox->setValue(value);
            }
            break;

        case FieldType::String:
            {
                this->lineEdit->setText(fieldValue);
            }
            break;
    }
}
