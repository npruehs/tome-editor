#include "fieldvaluewidget.h"

#include <limits>

#include "../Types/builtintype.h"
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
    this->setFieldType(BuiltInType::Integer);
}

FieldValueWidget::~FieldValueWidget()
{
}

QString FieldValueWidget::getFieldType() const
{
    return this->fieldType;
}

QString FieldValueWidget::getFieldValue() const
{
    ValueConverter valueConverter;
    QString fieldType = this->getFieldType();

    if (fieldType == BuiltInType::Boolean)
    {
        return valueConverter.BoolToString(this->checkBox->isChecked());
    }

    if (fieldType == BuiltInType::Color)
    {
        return this->colorDialog->currentColor().name(QColor::HexArgb);
    }

    if (fieldType == BuiltInType::Integer)
    {
         return this->spinBox->text();
    }

    if (fieldType == BuiltInType::Real)
    {
        return this->doubleSpinBox->text();
    }

    if (fieldType == BuiltInType::Reference)
    {
        return this->referenceComboBox->currentText();
    }

    if (fieldType == BuiltInType::String)
    {
        return this->lineEdit->text();
    }

    return QString();
}

void FieldValueWidget::setFieldType(const QString& fieldType)
{
    // Set new field type.
    this->fieldType = fieldType;

    // Update view.
    if (fieldType == BuiltInType::Boolean)
    {
        this->setCurrentWidget(this->checkBox);
        return;
    }

    if (fieldType == BuiltInType::Color)
    {
        this->setCurrentWidget(this->colorDialog);
        return;
    }

    if (fieldType == BuiltInType::Integer)
    {
        this->setCurrentWidget(this->spinBox);
        return;
    }

    if (fieldType == BuiltInType::Real)
    {
        this->setCurrentWidget(this->doubleSpinBox);
        return;
    }

    if (fieldType == BuiltInType::Reference)
    {
        this->setCurrentWidget(this->referenceComboBox);
        return;
    }

    if (fieldType == BuiltInType::String)
    {
        this->setCurrentWidget(this->lineEdit);
        return;
    }

    this->setCurrentWidget(0);
}

void FieldValueWidget::setFieldValue(const QString& fieldValue)
{
    ValueConverter valueConverter;

    if (fieldType == BuiltInType::Boolean)
    {
        bool value = valueConverter.StringToBool(fieldValue);
        this->checkBox->setChecked(value);
        return;
    }

    if (fieldType == BuiltInType::Color)
    {
        QColor color;
        color.setNamedColor(fieldValue);
        this->colorDialog->setCurrentColor(color);
        return;
    }

    if (fieldType == BuiltInType::Integer)
    {
        int value = fieldValue.toInt();
        this->spinBox->setValue(value);
        return;
    }

    if (fieldType == BuiltInType::Real)
    {
        double value = fieldValue.toDouble();
        this->doubleSpinBox->setValue(value);
        return;
    }

    if (fieldType == BuiltInType::Reference)
    {
        this->referenceComboBox->setCurrentText(fieldValue);
        return;
    }

    if (fieldType == BuiltInType::String)
    {
        this->lineEdit->setText(fieldValue);
        return;
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

void FieldValueWidget::focusInEvent(QFocusEvent* event)
{
    Q_UNUSED(event);

    if (this->currentWidget != 0)
    {
        this->currentWidget->setFocus();
    }
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
        this->currentWidget->setFocus();
    }
}
