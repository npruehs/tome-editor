#include "fieldvaluewidget.h"

#include <limits>

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
        case FieldType::Integer:
            this->lineEdit->hide();
            this->spinBox->show();
            this->doubleSpinBox->hide();
            break;

        case FieldType::Real:
            this->lineEdit->hide();
            this->spinBox->hide();
            this->doubleSpinBox->show();
            break;

        case FieldType::String:
            this->lineEdit->show();
            this->spinBox->hide();
            this->doubleSpinBox->hide();
            break;

        default:
            this->lineEdit->hide();
            this->spinBox->hide();
            this->doubleSpinBox->hide();
            break;
    }
}

void FieldValueWidget::setFieldValue(const QString& fieldValue)
{
    switch (this->getFieldType())
    {
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
