#include "requiredreferenceancestorfacet.h"

#include <QComboBox>

#include "../Model/facetcontext.h"
#include "../../Records/Controller/recordscontroller.h"
#include "../../Types/Model/builtintype.h"

using namespace Tome;


const QString RequiredReferenceAncestorFacet::FacetKey = "RequiredAncestor";


RequiredReferenceAncestorFacet::RequiredReferenceAncestorFacet()
{
}

QWidget* RequiredReferenceAncestorFacet::createWidget(const FacetContext& context) const
{
    QComboBox* comboBox = new QComboBox();
    QStringList recordIds = context.recordsController.getRecordIds();

    // Allow clearing the field.
    recordIds.push_front(QString());

    comboBox->addItems(recordIds);
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
    return FacetKey;
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
    QString recordId = value.toString();

    if ( !recordId.isEmpty() &&
         !requiredAncestor.isEmpty() &&
         context.recordsController.hasRecord(recordId) &&
         context.recordsController.hasRecord(requiredAncestor) &&
         !context.recordsController.isAncestorOf(requiredAncestor, recordId))
    {
        return tr("Value must be any %1.").arg(requiredAncestor);
    }

    return QString();
}
