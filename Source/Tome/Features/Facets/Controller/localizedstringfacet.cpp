#include "localizedstringfacet.h"

#include <QCheckBox>
#include <QVariant>

#include "../Model/facetcontext.h"
#include "../../Types/Model/builtintype.h"

using namespace Tome;


const QString LocalizedStringFacet::FacetKey = "Localized";


LocalizedStringFacet::LocalizedStringFacet()
{
}

QWidget*LocalizedStringFacet::createWidget(const FacetContext& context) const
{
    Q_UNUSED(context)
    return new QCheckBox();
}

const QVariant LocalizedStringFacet::getDefaultValue() const
{
    return true;
}

const QString LocalizedStringFacet::getDescriptionForValue(const QVariant facetValue) const
{
    bool localized = facetValue.toBool();
    return localized ? tr("Localized.") : QString();
}

const QString LocalizedStringFacet::getDisplayName() const
{
    return tr("Localized");
}

const QString LocalizedStringFacet::getKey() const
{
    return FacetKey;
}

const QString LocalizedStringFacet::getTargetType() const
{
    return BuiltInType::String;
}

const QVariant LocalizedStringFacet::getWidgetValue(QWidget* widget) const
{
    QCheckBox* checkBox = static_cast<QCheckBox*>(widget);
    return checkBox->isChecked();
}

void LocalizedStringFacet::setWidgetValue(QWidget* widget, const QVariant value) const
{
    QCheckBox* checkBox = static_cast<QCheckBox*>(widget);
    checkBox->setChecked(value.toBool());
}

QString LocalizedStringFacet::validateValue(const FacetContext& context, const QVariant value, const QVariant facetValue) const
{
    Q_UNUSED(context)
    Q_UNUSED(value)
    Q_UNUSED(facetValue)

    // Always succeeds.
    return QString();
}
