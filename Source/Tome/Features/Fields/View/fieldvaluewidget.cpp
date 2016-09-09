#include "fieldvaluewidget.h"

#include <limits>
#include <stdexcept>

#include "listwidget.h"
#include "mapwidget.h"
#include "vector2iwidget.h"
#include "vector2rwidget.h"
#include "vector3iwidget.h"
#include "vector3rwidget.h"
#include "../../Records/Controller/recordscontroller.h"
#include "../../Types/Controller/typescontroller.h"
#include "../../Types/Model/customtype.h"
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
    this->doubleSpinBox->setMinimum(-std::numeric_limits<float>::max());
    this->doubleSpinBox->setMaximum(std::numeric_limits<float>::max());
    this->doubleSpinBox->setDecimals(4);
    this->doubleSpinBox->setLocale(QLocale());
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

    this->vector2IWidget = new Vector2IWidget();
    this->addWidget(this->vector2IWidget);

    this->vector2RWidget = new Vector2RWidget();
    this->addWidget(this->vector2RWidget);

    this->vector3IWidget = new Vector3IWidget();
    this->addWidget(this->vector3IWidget);

    this->vector3RWidget = new Vector3RWidget();
    this->addWidget(this->vector3RWidget);

    this->mapWidget = new MapWidget(this->recordsController, this->typesController);
    this->addWidget(this->mapWidget);

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
        return this->comboBox->currentText();
    }

    if (this->fieldType == BuiltInType::Vector2I)
    {
        return this->vector2IWidget->getValue();
    }

    if (this->fieldType == BuiltInType::Vector2R)
    {
        return this->vector2RWidget->getValue();
    }

    if (this->fieldType == BuiltInType::Vector3I)
    {
        return this->vector3IWidget->getValue();
    }

    if (this->fieldType == BuiltInType::Vector3R)
    {
        return this->vector3RWidget->getValue();
    }

    // Custom type - or is it?
    if (!this->typesController.isCustomType(this->fieldType))
    {
        // Fall back to string.
        return this->lineEdit->text();
    }

    // Check custom data types.
    const CustomType& customType = this->typesController.getCustomType(this->fieldType);

    if (customType.isEnumeration())
    {
        return this->comboBox->currentText();
    }

    if (customType.isList())
    {
        return this->listWidget->getItems();
    }

    if (customType.isMap())
    {
        return this->mapWidget->getMap();
    }

    const QString errorMessage = "Unknown field type: " + this->fieldType;
    throw std::runtime_error(errorMessage.toStdString());
}

void FieldValueWidget::setFieldType(const QString& fieldType)
{
    if (fieldType.isEmpty())
    {
        return;
    }

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
        recordNames.push_front(QString());

        this->setEnumeration(recordNames);
        this->setCurrentWidget(this->comboBox);
        return;
    }

    if (this->fieldType == BuiltInType::Vector2I)
    {
        this->setCurrentWidget(this->vector2IWidget);
        return;
    }

    if (this->fieldType == BuiltInType::Vector2R)
    {
        this->setCurrentWidget(this->vector2RWidget);
        return;
    }

    if (this->fieldType == BuiltInType::Vector3I)
    {
        this->setCurrentWidget(this->vector3IWidget);
        return;
    }

    if (this->fieldType == BuiltInType::Vector3R)
    {
        this->setCurrentWidget(this->vector3RWidget);
        return;
    }

    // Custom type - or is it?
    if (!this->typesController.isCustomType(this->fieldType))
    {
        // Fall back to string.
        this->setCurrentWidget(this->lineEdit);
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

    if (customType.isList())
    {
        this->listWidget->setFieldType(customType.getItemType());
        this->setCurrentWidget(this->listWidget);
        return;
    }

    if (customType.isMap())
    {
        this->mapWidget->setKeyType(customType.getKeyType());
        this->mapWidget->setValueType(customType.getValueType());
        this->setCurrentWidget(this->mapWidget);
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

    if (this->fieldType == BuiltInType::Vector2I)
    {
        this->vector2IWidget->setValue(fieldValue);
        return;
    }

    if (this->fieldType == BuiltInType::Vector2R)
    {
        this->vector2RWidget->setValue(fieldValue);
        return;
    }

    if (this->fieldType == BuiltInType::Vector3I)
    {
        this->vector3IWidget->setValue(fieldValue);
        return;
    }

    if (this->fieldType == BuiltInType::Vector3R)
    {
        this->vector3RWidget->setValue(fieldValue);
        return;
    }

    // Custom type - or is it?
    if (!this->typesController.isCustomType(this->fieldType))
    {
        // Fall back to string.
        QString value = fieldValue.toString();
        this->lineEdit->setText(value);
        return;
    }

    // Check custom types.
    const CustomType& customType = this->typesController.getCustomType(this->fieldType);

    if (customType.isEnumeration())
    {
        this->comboBox->setCurrentText(fieldValue.toString());
        return;
    }

    if (customType.isList())
    {
        this->listWidget->setItems(fieldValue.toList());
        return;
    }

    if (customType.isMap())
    {
        this->mapWidget->setMap(fieldValue.toMap());
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

        // Select content for more convenient editing.
        if (this->fieldType == BuiltInType::Integer)
        {
            this->spinBox->selectAll();
        }
        else if (this->fieldType == BuiltInType::Real)
        {
            this->doubleSpinBox->selectAll();
        }
        else if (this->fieldType == BuiltInType::String)
        {
            this->lineEdit->selectAll();
        }
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
