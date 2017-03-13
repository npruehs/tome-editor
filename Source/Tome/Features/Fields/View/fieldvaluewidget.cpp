#include "fieldvaluewidget.h"

#include <limits>
#include <stdexcept>

#include <QMessageBox>

#include "filepickerwidget.h"
#include "listwidget.h"
#include "mapwidget.h"
#include "vector2iwidget.h"
#include "vector2rwidget.h"
#include "vector3iwidget.h"
#include "vector3rwidget.h"
#include "../../Facets/Controller/facet.h"
#include "../../Facets/Controller/facetscontroller.h"
#include "../../Records/Controller/recordscontroller.h"
#include "../../Types/Controller/typescontroller.h"
#include "../../Facets/Model/facetcontext.h"
#include "../../Types/Model/customtype.h"
#include "../../Types/Model/builtintype.h"
#include "../../../Util/memoryutils.h"

using namespace Tome;


FieldValueWidget::FieldValueWidget(FacetsController& facetsController,
                                   ProjectController& projectController,
                                   RecordsController& recordsController,
                                   TypesController& typesController,
                                   QWidget *parent) :
    QWidget(parent),
    currentWidget(0),
    facetsController(facetsController),
    projectController(projectController),
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

    this->listWidget = new ListWidget(this->facetsController, this->projectController, this->recordsController, this->typesController);
    this->addWidget(this->listWidget);

    this->vector2IWidget = new Vector2IWidget();
    this->addWidget(this->vector2IWidget);

    this->vector2RWidget = new Vector2RWidget();
    this->addWidget(this->vector2RWidget);

    this->vector3IWidget = new Vector3IWidget();
    this->addWidget(this->vector3IWidget);

    this->vector3RWidget = new Vector3RWidget();
    this->addWidget(this->vector3RWidget);

    this->mapWidget = new MapWidget(this->facetsController, this->projectController, this->recordsController, this->typesController);
    this->addWidget(this->mapWidget);

    this->filePicker = new FilePickerWidget(this->facetsController, this->projectController);
    this->addWidget(this->filePicker);

    // Add error label.
    this->errorLabel = new QLabel();
    this->addWidget(this->errorLabel);

    // Connect to signals.
    connect(
                this->colorDialog,
                SIGNAL(currentColorChanged(QColor)),
                SLOT(onColorDialogCurrentColorChanged(QColor))
                );

    connect(
                this->doubleSpinBox,
                SIGNAL(valueChanged(double)),
                SLOT(onDoubleSpinBoxValueChanged(double))
                );

    connect(
                this->lineEdit,
                SIGNAL(textChanged(QString)),
                SLOT(onLineEditTextChanged(QString))
                );

    connect(
                this->comboBox,
                SIGNAL(currentIndexChanged(QString)),
                SLOT(onComboBoxCurrentIndexChanged(QString))
                );

    connect(
                this->spinBox,
                SIGNAL(valueChanged(int)),
                SLOT(onSpinBoxValueChanged(int))
                );

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
    return this->getFieldValueForType(this->fieldType);
}

void FieldValueWidget::setFieldType(const QString& fieldType)
{
    if (fieldType.isEmpty())
    {
        return;
    }

    // Set new field type.
    this->fieldType = fieldType;

    // Update view.
    this->selectWidgetForType(fieldType);
}

void FieldValueWidget::setFieldValue(const QVariant& fieldValue)
{
    this->setFieldValueForType(fieldValue, this->fieldType);
}

QString FieldValueWidget::validate()
{
    const QString fieldType = this->getFieldType();
    const QVariant fieldValue = this->getFieldValue();

    return this->facetsController.validateFieldValue(fieldType, fieldValue);
}

void FieldValueWidget::focusInEvent(QFocusEvent* event)
{
    Q_UNUSED(event)

    if (this->currentWidget != 0)
    {
        this->currentWidget->setFocus();

        // Select content for more convenient editing.
        CustomType customType;

        if (this->typesController.isCustomType(this->fieldType))
        {
            customType = this->typesController.getCustomType(this->fieldType);
        }

        if (this->fieldType == BuiltInType::Integer || customType.getBaseType() == BuiltInType::Integer)
        {
            this->spinBox->selectAll();
        }
        else if (this->fieldType == BuiltInType::Real || customType.getBaseType() == BuiltInType::Real)
        {
            this->doubleSpinBox->selectAll();
        }
        else if (this->fieldType == BuiltInType::String || customType.getBaseType() == BuiltInType::String)
        {
            this->lineEdit->selectAll();
        }
    }
}

void FieldValueWidget::onColorDialogCurrentColorChanged(const QColor &color)
{
    Q_UNUSED(color)
    this->updateErrorLabel();
}

void FieldValueWidget::onDoubleSpinBoxValueChanged(double d)
{
    Q_UNUSED(d)
    this->updateErrorLabel();
}

void FieldValueWidget::onLineEditTextChanged(const QString &text)
{
    Q_UNUSED(text)
    this->updateErrorLabel();
}

void FieldValueWidget::onComboBoxCurrentIndexChanged(const QString &text)
{
    Q_UNUSED(text)
    this->updateErrorLabel();
}

void FieldValueWidget::onSpinBoxValueChanged(int i)
{
    Q_UNUSED(i)
    this->updateErrorLabel();
}

void FieldValueWidget::addWidget(QWidget* widget)
{
    if (widget != 0)
    {
        widget->hide();
        this->layout->addWidget(widget);
    }
}

QVariant FieldValueWidget::getFieldValueForType(const QString& typeName) const
{
    // Check built-in types.
    if (typeName == BuiltInType::Boolean)
    {
        return this->checkBox->isChecked();
    }

    if (typeName == BuiltInType::Color)
    {
        return this->colorDialog->currentColor();
    }

    if (typeName == BuiltInType::File)
    {
        return this->filePicker->getFileName();
    }

    if (typeName == BuiltInType::Integer)
    {
        return this->spinBox->text();
    }

    if (typeName == BuiltInType::Real)
    {
        return this->doubleSpinBox->text();
    }

    if (typeName == BuiltInType::String)
    {
        return this->lineEdit->text();
    }

    if (typeName == BuiltInType::Reference)
    {
        return this->comboBox->currentText();
    }

    if (typeName == BuiltInType::Vector2I)
    {
        return this->vector2IWidget->getValue();
    }

    if (typeName == BuiltInType::Vector2R)
    {
        return this->vector2RWidget->getValue();
    }

    if (typeName == BuiltInType::Vector3I)
    {
        return this->vector3IWidget->getValue();
    }

    if (typeName == BuiltInType::Vector3R)
    {
        return this->vector3RWidget->getValue();
    }

    // Custom type - or is it?
    if (!this->typesController.isCustomType(typeName))
    {
        // Fall back to string.
        return this->lineEdit->text();
    }

    // Check custom data types.
    const CustomType& customType = this->typesController.getCustomType(typeName);

    if (customType.isDerivedType())
    {
        return this->getFieldValueForType(customType.getBaseType());
    }

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

    const QString errorMessage = "Unknown field type: " + typeName;
    throw std::runtime_error(errorMessage.toStdString());
}

void FieldValueWidget::selectWidgetForType(const QString& typeName)
{
    // Check built-in types.
    if (typeName == BuiltInType::Boolean)
    {
        this->setCurrentWidget(this->checkBox);
        return;
    }

    if (typeName == BuiltInType::Color)
    {
        this->setCurrentWidget(this->colorDialog);
        return;
    }

    if (typeName == BuiltInType::File)
    {
        this->filePicker->setFieldType(this->fieldType);
        this->setCurrentWidget(this->filePicker);
        return;
    }

    if (typeName == BuiltInType::Integer)
    {
        this->setCurrentWidget(this->spinBox);
        return;
    }

    if (typeName == BuiltInType::Real)
    {
        this->setCurrentWidget(this->doubleSpinBox);
        return;
    }

    if (typeName == BuiltInType::String)
    {
        this->setCurrentWidget(this->lineEdit);
        return;
    }

    if (typeName == BuiltInType::Reference)
    {
        QVariantList recordIds = this->recordsController.getRecordIds();

        // Only show allowed record references.
        QStringList references;

        for (const QVariant recordId : recordIds)
        {
            const QString& validationError =
                    this->facetsController.validateFieldValue(this->getFieldType(), recordId);

            if (validationError.isEmpty())
            {
                references.push_back(recordId.toString());
            }
        }

        // Allow clearing the field.
        references.push_front(QString());

        this->setEnumeration(references);
        this->setCurrentWidget(this->comboBox);
        return;
    }

    if (typeName == BuiltInType::Vector2I)
    {
        this->setCurrentWidget(this->vector2IWidget);
        return;
    }

    if (typeName == BuiltInType::Vector2R)
    {
        this->setCurrentWidget(this->vector2RWidget);
        return;
    }

    if (typeName == BuiltInType::Vector3I)
    {
        this->setCurrentWidget(this->vector3IWidget);
        return;
    }

    if (typeName == BuiltInType::Vector3R)
    {
        this->setCurrentWidget(this->vector3RWidget);
        return;
    }

    // Custom type - or is it?
    if (!this->typesController.isCustomType(typeName))
    {
        // Fall back to string.
        this->setCurrentWidget(this->lineEdit);
        return;
    }

    // Update view - check custom types.
    const CustomType& customType = this->typesController.getCustomType(typeName);

    if (customType.isDerivedType())
    {
        this->selectWidgetForType(customType.getBaseType());
        return;
    }

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

    const QString errorMessage = "Unknown field type: " + typeName;
    throw std::runtime_error(errorMessage.toStdString());
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

void FieldValueWidget::setEnumeration(const QStringList& enumeration)
{
    this->comboBox->clear();
    this->comboBox->addItems(enumeration);
}

void FieldValueWidget::setFieldValueForType(const QVariant& fieldValue, const QString& typeName)
{
    // Check built-in types.
    if (typeName == BuiltInType::Boolean)
    {
        bool value = fieldValue.toBool();
        this->checkBox->setChecked(value);
        return;
    }

    if (typeName == BuiltInType::Color)
    {
        QColor color = fieldValue.value<QColor>();
        this->colorDialog->setCurrentColor(color);
        return;
    }

    if (typeName == BuiltInType::File)
    {
        QString fileName = fieldValue.toString();
        this->filePicker->setFileName(fileName);
        return;
    }

    if (typeName == BuiltInType::Integer)
    {
        int value = fieldValue.toInt();
        this->spinBox->setValue(value);
        return;
    }

    if (typeName == BuiltInType::Real)
    {
        double value = fieldValue.toDouble();
        this->doubleSpinBox->setValue(value);
        return;
    }

    if (typeName == BuiltInType::String)
    {
        QString value = fieldValue.toString();
        this->lineEdit->setText(value);
        return;
    }

    if (typeName == BuiltInType::Reference)
    {
        this->comboBox->setCurrentText(fieldValue.toString());
        return;
    }

    if (typeName == BuiltInType::Vector2I)
    {
        this->vector2IWidget->setValue(fieldValue);
        return;
    }

    if (typeName == BuiltInType::Vector2R)
    {
        this->vector2RWidget->setValue(fieldValue);
        return;
    }

    if (typeName == BuiltInType::Vector3I)
    {
        this->vector3IWidget->setValue(fieldValue);
        return;
    }

    if (typeName == BuiltInType::Vector3R)
    {
        this->vector3RWidget->setValue(fieldValue);
        return;
    }

    // Custom type - or is it?
    if (!this->typesController.isCustomType(typeName))
    {
        // Fall back to string.
        QString value = fieldValue.toString();
        this->lineEdit->setText(value);
        return;
    }

    // Check custom types.
    const CustomType& customType = this->typesController.getCustomType(typeName);

    if (customType.isDerivedType())
    {
        this->setFieldValueForType(fieldValue, customType.getBaseType());
        return;
    }

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

    const QString errorMessage = "Unknown field type: " + typeName;
    throw std::runtime_error(errorMessage.toStdString());
}

void FieldValueWidget::updateErrorLabel()
{
    QString validationError = this->validate();

    if (validationError.isEmpty())
    {
        this->errorLabel->clear();
        this->errorLabel->hide();
    }
    else
    {
        QString errorMessage = QString("<font color=\"#FF0000\">%1</font>").arg(validationError);
        this->errorLabel->setText(errorMessage);
        this->errorLabel->show();
    }
}
