#include "maximumintegervaluefacet.h"

#include <limits>

#include <QSpinBox>

#include "../../Types/Model/builtintype.h"

using namespace Tome;


MaximumIntegerValueFacet::MaximumIntegerValueFacet()
{
}

QWidget* MaximumIntegerValueFacet::createWidget() const
{
    QSpinBox* spinBox = new QSpinBox();
    spinBox->setMinimum(std::numeric_limits<int>::min());
    spinBox->setMaximum(std::numeric_limits<int>::max());
    return spinBox;
}

const QVariant MaximumIntegerValueFacet::getDefaultValue() const
{
    return std::numeric_limits<int>::max();
}

const QString MaximumIntegerValueFacet::getDescriptionForValue(const QVariant facetValue) const
{
    return tr("Maximum value is %1.").arg(facetValue.toString());
}

const QString MaximumIntegerValueFacet::getTargetType() const
{
    return BuiltInType::Integer;
}

const QVariant MaximumIntegerValueFacet::getWidgetValue(QWidget* widget) const
{
    QSpinBox* spinBox = static_cast<QSpinBox*>(widget);
    return spinBox->value();
}

const QString MaximumIntegerValueFacet::getDisplayName() const
{
    return tr("Maximum Value");
}

const QString MaximumIntegerValueFacet::getKey() const
{
    return "Max";
}

void MaximumIntegerValueFacet::setWidgetValue(QWidget* widget, const QVariant value) const
{
    QSpinBox* spinBox = static_cast<QSpinBox*>(widget);
    spinBox->setValue(value.toInt());
}

QString MaximumIntegerValueFacet::validateValue(const QVariant value, const QVariant facetValue) const
{
    if (value.toInt() > facetValue.toInt())
    {
        return tr("Value must not be greater than %1.").arg(facetValue.toString());
    }

    return QString();
}
