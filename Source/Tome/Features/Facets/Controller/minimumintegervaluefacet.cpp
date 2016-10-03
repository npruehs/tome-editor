#include "minimumintegervaluefacet.h"

#include <limits>

#include <QSpinBox>

#include "../../Types/Model/builtintype.h"

using namespace Tome;


MinimumIntegerValueFacet::MinimumIntegerValueFacet()
{

}

QWidget* MinimumIntegerValueFacet::createWidget() const
{
    QSpinBox* spinBox = new QSpinBox();
    spinBox->setMinimum(std::numeric_limits<int>::min());
    spinBox->setMaximum(std::numeric_limits<int>::max());
    return spinBox;
}

const QVariant MinimumIntegerValueFacet::getDefaultValue() const
{
    return std::numeric_limits<int>::min();
}

const QString MinimumIntegerValueFacet::getDescriptionForValue(const QVariant facetValue) const
{
    return tr("Minimum value is %1.").arg(facetValue.toString());
}

const QString MinimumIntegerValueFacet::getDisplayName() const
{
    return tr("Minimum Value");
}

const QString MinimumIntegerValueFacet::getKey() const
{
    return "Min";
}

const QString MinimumIntegerValueFacet::getTargetType() const
{
    return BuiltInType::Integer;
}

const QVariant MinimumIntegerValueFacet::getWidgetValue(QWidget* widget) const
{
    QSpinBox* spinBox = static_cast<QSpinBox*>(widget);
    return spinBox->value();
}

void MinimumIntegerValueFacet::setWidgetValue(QWidget* widget, const QVariant value) const
{
    QSpinBox* spinBox = static_cast<QSpinBox*>(widget);
    spinBox->setValue(value.toInt());
}

QString MinimumIntegerValueFacet::validateValue(const QVariant value, const QVariant facetValue) const
{
    if (value.toInt() < facetValue.toInt())
    {
        return tr("Value must not be less than %1.").arg(facetValue.toString());
    }

    return QString();
}
