#include "minimumrealvaluefacet.h"

#include <limits>

#include <QSpinBox>

#include "../Model/facetcontext.h"
#include "../../Types/Model/builtintype.h"

using namespace Tome;


MinimumRealValueFacet::MinimumRealValueFacet()
{

}

QWidget* MinimumRealValueFacet::createWidget(const FacetContext& context) const
{
    Q_UNUSED(context)

    QDoubleSpinBox* doubleSpinBox = new QDoubleSpinBox();
    doubleSpinBox->setMinimum(-std::numeric_limits<float>::max());
    doubleSpinBox->setMaximum(std::numeric_limits<float>::max());
    doubleSpinBox->setDecimals(4);
    return doubleSpinBox;
}

const QVariant MinimumRealValueFacet::getDefaultValue() const
{
    return -std::numeric_limits<float>::max();
}

const QString MinimumRealValueFacet::getDescriptionForValue(const QVariant facetValue) const
{
    return tr("Minimum value is %1.").arg(facetValue.toString());
}

const QString MinimumRealValueFacet::getDisplayName() const
{
    return tr("Minimum Value");
}

const QString MinimumRealValueFacet::getKey() const
{
    return "Min";
}

const QString MinimumRealValueFacet::getTargetType() const
{
    return BuiltInType::Real;
}

const QVariant MinimumRealValueFacet::getWidgetValue(QWidget* widget) const
{
    QDoubleSpinBox* doubleSpinBox = static_cast<QDoubleSpinBox*>(widget);
    return doubleSpinBox->value();
}

void MinimumRealValueFacet::setWidgetValue(QWidget* widget, const QVariant value) const
{
    QDoubleSpinBox* doubleSpinBox = static_cast<QDoubleSpinBox*>(widget);
    doubleSpinBox->setValue(value.toFloat());
}

QString MinimumRealValueFacet::validateValue(const FacetContext& context, const QVariant value, const QVariant facetValue) const
{
    Q_UNUSED(context)

    if (value.toFloat() < facetValue.toFloat())
    {
        return tr("Value must not be less than %1.").arg(facetValue.toString());
    }

    return QString();
}
