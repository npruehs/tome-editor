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
    const RecordList records = context.recordsController.getRecords();

    // Allow clearing the field.
    comboBox->addItem(QString(), QVariant());

    for (const Record& record : records)
    {
        comboBox->addItem(record.displayName, record.id);
    }

    return comboBox;
}

const QVariant RequiredReferenceAncestorFacet::getDefaultValue() const
{
    return QVariant();
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
    return comboBox->currentData();
}

void RequiredReferenceAncestorFacet::setWidgetValue(QWidget* widget, const QVariant value) const
{
    QComboBox* comboBox = static_cast<QComboBox*>(widget);

    for (int i = 0; i < comboBox->count(); ++i)
    {
        if (comboBox->itemData(i) == value)
        {
            comboBox->setCurrentIndex(i);
            return;
        }
    }
}

QString RequiredReferenceAncestorFacet::validateValue(const FacetContext& context, const QVariant value, const QVariant facetValue) const
{
    QVariant requiredAncestor = facetValue;
    QVariant recordId = value;

    if ( !recordId.isNull() &&
         !requiredAncestor.isNull() &&
         context.recordsController.hasRecord(recordId) &&
         context.recordsController.hasRecord(requiredAncestor) &&
         !context.recordsController.isAncestorOf(requiredAncestor, recordId))
    {
        return tr("Value must be any %1.").arg(requiredAncestor.toString());
    }

    return QString();
}
