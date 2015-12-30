#include "fieldvaluewidget.h"

#include <limits>

#include "../Values/valueconverter.h"

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
    this->spinBox->setMinimum(std::numeric_limits<int>::min());
    this->spinBox->setMaximum(std::numeric_limits<int>::max());
    this->layout->addWidget(this->spinBox);

    this->doubleSpinBox = new QDoubleSpinBox();
    this->doubleSpinBox->setMinimum(std::numeric_limits<float>::min());
    this->doubleSpinBox->setMaximum(std::numeric_limits<float>::max());
    this->doubleSpinBox->setDecimals(3);
    this->layout->addWidget(this->doubleSpinBox);

    this->checkBox = new QCheckBox("Value");
    this->layout->addWidget(this->checkBox);

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
    ValueConverter valueConverter;

    switch (this->getFieldType())
    {
        case FieldType::Boolean:
            return valueConverter.BoolToString(this->checkBox->isChecked());

        case FieldType::Integer:
            return this->spinBox->text();

        case FieldType::Real:
            return this->doubleSpinBox->text();

        case FieldType::String:
            return this->lineEdit->text();

        default:
            return QString();
    }
}

void FieldValueWidget::setFieldType(const FieldType::FieldType& fieldType)
{
    // Set new field type.
    this->fieldType = fieldType;

    // Update view.
    switch (fieldType)
    {
        case FieldType::Boolean:
            this->checkBox->show();
            this->doubleSpinBox->hide();
            this->lineEdit->hide();
            this->spinBox->hide();
            break;

        case FieldType::Integer:
            this->checkBox->hide();
            this->doubleSpinBox->hide();
            this->lineEdit->hide();
            this->spinBox->show();
            break;

        case FieldType::Real:
            this->checkBox->hide();
            this->doubleSpinBox->show();
            this->lineEdit->hide();
            this->spinBox->hide();
            break;

        case FieldType::String:
            this->checkBox->hide();
            this->doubleSpinBox->hide();
            this->lineEdit->show();
            this->spinBox->hide();
            break;

        default:
            this->checkBox->hide();
            this->doubleSpinBox->hide();
            this->lineEdit->hide();
            this->spinBox->hide();
            break;
    }
}

void FieldValueWidget::setFieldValue(const QString& fieldValue)
{
    ValueConverter valueConverter;


    switch (this->getFieldType())
    {
        case FieldType::Boolean:
            {
                bool value = valueConverter.StringToBool(fieldValue);
                this->checkBox->setChecked(value);
            }
            break;

        case FieldType::Integer:
            {
                int value = fieldValue.toInt();
                this->spinBox->setValue(value);
            }
            break;

        case FieldType::Real:
            {
                double value = fieldValue.toDouble();
                this->doubleSpinBox->setValue(value);
            }
            break;

        case FieldType::String:
            {
                this->lineEdit->setText(fieldValue);
            }
            break;

        default:
            break;
    }
}
