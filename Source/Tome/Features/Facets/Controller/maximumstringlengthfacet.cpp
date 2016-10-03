#include "maximumstringlengthfacet.h"

#include <limits>

#include <QSpinBox>

#include "../Model/facetcontext.h"
#include "../../Types/Model/builtintype.h"

using namespace Tome;


MaximumStringLengthFacet::MaximumStringLengthFacet()
{

}

QWidget* MaximumStringLengthFacet::createWidget(const FacetContext& context) const
{
    Q_UNUSED(context)

    QSpinBox* spinBox = new QSpinBox();
    spinBox->setMinimum(0);
    spinBox->setMaximum(std::numeric_limits<int>::max());
    return spinBox;
}

const QVariant MaximumStringLengthFacet::getDefaultValue() const
{
    return 0;
}

const QString MaximumStringLengthFacet::getDescriptionForValue(const QVariant facetValue) const
{
    int maximumLength = facetValue.toInt();
    return maximumLength > 0
            ? tr("Maximum length is %1.").arg(facetValue.toString())
            : tr("Unlimited length.");
}

const QString MaximumStringLengthFacet::getDisplayName() const
{
    return tr("Maximum Length (0 = Unlimited)");
}

const QString MaximumStringLengthFacet::getKey() const
{
    return "MaxLength";
}

const QString MaximumStringLengthFacet::getTargetType() const
{
    return BuiltInType::String;
}

const QVariant MaximumStringLengthFacet::getWidgetValue(QWidget* widget) const
{
    QSpinBox* spinBox = static_cast<QSpinBox*>(widget);
    return spinBox->value();
}

void MaximumStringLengthFacet::setWidgetValue(QWidget* widget, const QVariant value) const
{
    QSpinBox* spinBox = static_cast<QSpinBox*>(widget);
    spinBox->setValue(value.toInt());
}

QString MaximumStringLengthFacet::validateValue(const FacetContext& context, const QVariant value, const QVariant facetValue) const
{
    Q_UNUSED(context)

    int maxLength = facetValue.toInt();

    if (maxLength > 0 && value.toString().length() > maxLength)
    {
        return tr("Value must not be longer than %1 characters.").arg(maxLength);
    }

    return QString();
}
