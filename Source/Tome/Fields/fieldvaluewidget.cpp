#include "fieldvaluewidget.h"

#include <limits>

#include "../Values/valueconverter.h"

using namespace Tome;


FieldValueWidget::FieldValueWidget(QWidget *parent) :
    QWidget(parent),
    currentWidget(0)
{
    // Create layout.
    this->layout = new QVBoxLayout();

    // Add field value widgets.
    this->lineEdit = new QLineEdit();
    this->addWidget(this->lineEdit);

    this->spinBox = new QSpinBox();
    this->spinBox->setMinimum(std::numeric_limits<int>::min());
    this->spinBox->setMaximum(std::numeric_limits<int>::max());
    this->addWidget(this->spinBox);

    this->doubleSpinBox = new QDoubleSpinBox();
    this->doubleSpinBox->setMinimum(std::numeric_limits<float>::min());
    this->doubleSpinBox->setMaximum(std::numeric_limits<float>::max());
    this->doubleSpinBox->setDecimals(3);
    this->addWidget(this->doubleSpinBox);

    this->checkBox = new QCheckBox("Value");
    this->addWidget(this->checkBox);

    this->colorDialog = new QColorDialog();
    this->colorDialog->setOptions(QColorDialog::ShowAlphaChannel | QColorDialog::NoButtons);
    this->addWidget(this->colorDialog);

    this->referenceComboBox = new QComboBox();
    this->addWidget(this->referenceComboBox);

    this->setLayout(this->layout);
    this->layout->setContentsMargins(0, 0, 0, 0);

    // Pre-select type.
    this->setFieldType(FieldType::Integer);
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

        case FieldType::Color:
            return this->colorDialog->currentColor().name(QColor::HexArgb);

        case FieldType::Integer:
            return this->spinBox->text();

        case FieldType::Real:
            return this->doubleSpinBox->text();

        case FieldType::Reference:
            return this->referenceComboBox->currentText();

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
        case FieldType::Color:
            this->setCurrentWidget(this->colorDialog);
            break;

        case FieldType::Boolean:
            this->setCurrentWidget(this->checkBox);
            break;

        case FieldType::Integer:
            this->setCurrentWidget(this->spinBox);
            break;

        case FieldType::Real:
            this->setCurrentWidget(this->doubleSpinBox);
            break;

        case FieldType::Reference:
            this->setCurrentWidget(this->referenceComboBox);
            break;

        case FieldType::String:
            this->setCurrentWidget(this->lineEdit);
            break;

        default:
            this->setCurrentWidget(0);
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

        case FieldType::Color:
        {
            QColor color;
            color.setNamedColor(fieldValue);
            this->colorDialog->setCurrentColor(color);
        }
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

        case FieldType::Reference:
            {
                this->referenceComboBox->setCurrentText(fieldValue);
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

void FieldValueWidget::setRecordNames(const QStringList& recordNames)
{
    this->referenceComboBox->clear();

    // Allow clearing the field.
    this->referenceComboBox->addItem(QString());

    // Add all available records.
    this->referenceComboBox->addItems(recordNames);
}

void FieldValueWidget::addWidget(QWidget* widget)
{
    if (widget != 0)
    {
        widget->hide();
         this->layout->addWidget(widget);
    }
}

void FieldValueWidget::setCurrentWidget(QWidget* widget)
{
    if (this->currentWidget != 0)
    {
        this->currentWidget->hide();
    }

    this->currentWidget = widget;

    if (this->currentWidget != 0)
    {
        this->currentWidget->show();
    }
}
