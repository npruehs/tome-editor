#include "requiredreferenceancestorfacet.h"

#include <QComboBox>

#include "../Model/facetcontext.h"
#include "../../Records/Controller/recordscontroller.h"
#include "../../Types/Model/builtintype.h"

using namespace Tome;


RequiredReferenceAncestorFacet::RequiredReferenceAncestorFacet()
{
}

QWidget* RequiredReferenceAncestorFacet::createWidget(const FacetContext& context) const
{
    QComboBox* comboBox = new QComboBox();
    QStringList recordNames = context.recordsController.getRecordNames();

    // Allow clearing the field.
    recordNames.push_front(QString());

    comboBox->addItems(recordNames);
    return comboBox;
}

const QVariant RequiredReferenceAncestorFacet::getDefaultValue() const
{
    return QString();
}

const QString RequiredReferenceAncestorFacet::getDescriptionForValue(const QVariant facetValue) const
{
    QString requiredAncestor = facetValue.toString();
    return requiredAncestor.length() > 0
            ? tr("Must be any %1.").arg(requiredAncestor)
            : QString();
}

const QString RequiredReferenceAncestorFacet::getDisplayName() const
{
    return tr("Required Ancestor (Empty = No Restriction)");
}

const QString RequiredReferenceAncestorFacet::getKey() const
{
    return "RequiredAncestor";
}

const QString RequiredReferenceAncestorFacet::getTargetType() const
{
    return BuiltInType::Reference;
}

const QVariant RequiredReferenceAncestorFacet::getWidgetValue(QWidget* widget) const
{
    QComboBox* comboBox = static_cast<QComboBox*>(widget);
    return comboBox->currentText();
}

void RequiredReferenceAncestorFacet::setWidgetValue(QWidget* widget, const QVariant value) const
{
    QComboBox* comboBox = static_cast<QComboBox*>(widget);
    comboBox->setCurrentText(value.toString());
}

QString RequiredReferenceAncestorFacet::validateValue(const FacetContext& context, const QVariant value, const QVariant facetValue) const
{
    QString requiredAncestor = facetValue.toString();

    if (!requiredAncestor.isEmpty() && !context.recordsController.isAncestorOf(requiredAncestor, value.toString()))
    {
        return tr("Value must be any %1.").arg(requiredAncestor);
    }

    return QString();
}
