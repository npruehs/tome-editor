#include "fieldvaluewidget.h"

#include <limits>
#include <stdexcept>

#include "../../Types/Model/builtintype.h"
#include "../../../Util/memoryutils.h"

using namespace Tome;


FieldValueWidget::FieldValueWidget(RecordsController& recordsController, TypesController& typesController, QWidget *parent) :
    QWidget(parent),
    currentWidget(0),
    recordsController(recordsController),
    typesController(typesController)
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

    this->comboBox = new QComboBox();
    this->addWidget(this->comboBox);

    this->listWidget = new ListWidget(this->recordsController, this->typesController);
    this->addWidget(this->listWidget);

    // Set layout.
    this->setLayout(this->layout);
    this->layout->setContentsMargins(0, 0, 0, 0);

    // Pre-select type.
    this->setFieldType(BuiltInType::Integer);
}

FieldValueWidget::~FieldValueWidget()
{
    deleteLayout(this->layout);
}

QString FieldValueWidget::getFieldType() const
{
    return this->fieldType;
}

QVariant FieldValueWidget::getFieldValue() const
{
    // Check built-in types.
    if (this->fieldType == BuiltInType::Boolean)
    {
        return this->checkBox->isChecked();
    }

    if (this->fieldType == BuiltInType::Color)
    {
        return this->colorDialog->currentColor();
    }

    if (this->fieldType == BuiltInType::Integer)
    {
        return this->spinBox->text();
    }

    if (this->fieldType == BuiltInType::Real)
    {
        return this->doubleSpinBox->text();
    }

    if (this->fieldType == BuiltInType::String)
    {
        return this->lineEdit->text();
    }

    if (this->fieldType == BuiltInType::Reference)
    {
        return this->lineEdit->text();
    }

    // Check custom data types.
    const CustomType& customType = this->typesController.getCustomType(this->fieldType);

    if (customType.isEnumeration())
    {
        return this->comboBox->currentText();
    }

    const QString errorMessage = "Unknown field type: " + this->fieldType;
    throw std::runtime_error(errorMessage.toStdString());
}

void FieldValueWidget::setFieldType(const QString& fieldType)
{
    // Set new field type.
    this->fieldType = fieldType;

    // Update view - check built-in types.
    if (this->fieldType == BuiltInType::Boolean)
    {
        this->setCurrentWidget(this->checkBox);
        return;
    }

    if (this->fieldType == BuiltInType::Color)
    {
        this->setCurrentWidget(this->colorDialog);
        return;
    }

    if (this->fieldType == BuiltInType::Integer)
    {
        this->setCurrentWidget(this->spinBox);
        return;
    }

    if (this->fieldType == BuiltInType::Real)
    {
        this->setCurrentWidget(this->doubleSpinBox);
        return;
    }

    if (this->fieldType == BuiltInType::String)
    {
        this->setCurrentWidget(this->lineEdit);
        return;
    }

    if (this->fieldType == BuiltInType::Reference)
    {
        QStringList recordNames = this->recordsController.getRecordNames();

        // Allow clearing the field.
        recordNames << QString();

        this->setEnumeration(recordNames);
        this->setCurrentWidget(this->comboBox);
        return;
    }

    // Update view - check custom types.
    const CustomType& customType = this->typesController.getCustomType(this->fieldType);

    if (customType.isEnumeration())
    {
        this->setEnumeration(customType.getEnumeration());
        this->setCurrentWidget(this->comboBox);
        return;
    }

    const QString errorMessage = "Unknown field type: " + this->fieldType;
    throw std::runtime_error(errorMessage.toStdString());
}

void FieldValueWidget::setFieldValue(const QVariant& fieldValue)
{
    // Check built-in types.
    if (this->fieldType == BuiltInType::Boolean)
    {
        bool value = fieldValue.toBool();
        this->checkBox->setChecked(value);
        return;
    }

    if (this->fieldType == BuiltInType::Color)
    {
        QColor color = fieldValue.value<QColor>();
        this->colorDialog->setCurrentColor(color);
        return;
    }

    if (this->fieldType == BuiltInType::Integer)
    {
        int value = fieldValue.toInt();
        this->spinBox->setValue(value);
        return;
    }

    if (this->fieldType == BuiltInType::Real)
    {
        double value = fieldValue.toDouble();
        this->doubleSpinBox->setValue(value);
        return;
    }

    if (this->fieldType == BuiltInType::String)
    {
        QString value = fieldValue.toString();
        this->lineEdit->setText(value);
        return;
    }

    if (this->fieldType == BuiltInType::Reference)
    {
        this->comboBox->setCurrentText(fieldValue.toString());
        return;
    }

    // Check custom types.
    const CustomType& customType = this->typesController.getCustomType(this->fieldType);

    if (customType.isEnumeration())
    {
        this->comboBox->setCurrentText(fieldValue.toString());
        return;
    }

    const QString errorMessage = "Unknown field type: " + this->fieldType;
    throw std::runtime_error(errorMessage.toStdString());
}

void FieldValueWidget::setEnumeration(const QStringList& enumeration)
{
    this->comboBox->clear();
    this->comboBox->addItems(enumeration);
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
