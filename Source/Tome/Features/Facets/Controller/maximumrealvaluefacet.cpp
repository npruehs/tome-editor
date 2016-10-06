#include "maximumrealvaluefacet.h"

#include <limits>

#include <QDoubleSpinBox>

#include "../Model/facetcontext.h"
#include "../../Types/Model/builtintype.h"

using namespace Tome;


MaximumRealValueFacet::MaximumRealValueFacet()
{

}

QWidget* MaximumRealValueFacet::createWidget(const FacetContext& context) const
{
    Q_UNUSED(context)

    QDoubleSpinBox* doubleSpinBox = new QDoubleSpinBox();
    doubleSpinBox->setMinimum(-std::numeric_limits<float>::max());
    doubleSpinBox->setMaximum(std::numeric_limits<float>::max());
    doubleSpinBox->setDecimals(4);
    return doubleSpinBox;
}

const QVariant MaximumRealValueFacet::getDefaultValue() const
{
    return std::numeric_limits<float>::max();
}

const QString MaximumRealValueFacet::getDescriptionForValue(const QVariant facetValue) const
{
    return tr("Maximum value is %1.").arg(facetValue.toString());
}

const QString MaximumRealValueFacet::getDisplayName() const
{
    return tr("Maximum Value");
}

const QString MaximumRealValueFacet::getKey() const
{
    return "Max";
}

const QString MaximumRealValueFacet::getTargetType() const
{
    return BuiltInType::Real;
}

const QVariant MaximumRealValueFacet::getWidgetValue(QWidget* widget) const
{
    QDoubleSpinBox* doubleSpinBox = static_cast<QDoubleSpinBox*>(widget);
    return doubleSpinBox->value();
}

void MaximumRealValueFacet::setWidgetValue(QWidget* widget, const QVariant value) const
{
    QDoubleSpinBox* doubleSpinBox = static_cast<QDoubleSpinBox*>(widget);
    doubleSpinBox->setValue(value.toFloat());
}

QString MaximumRealValueFacet::validateValue(const FacetContext& context, const QVariant value, const QVariant facetValue) const
{
    Q_UNUSED(context)

    if (value.toFloat() > facetValue.toFloat())
    {
        return tr("Value must not be greater than %1.").arg(facetValue.toString());
    }

    return QString();
}
