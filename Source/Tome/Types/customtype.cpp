#include "customtype.h"

using namespace Tome;


const QString CustomType::RestrictionEnumeration = "Enumeration";
const QString CustomType::RestrictionItemType = "ItemType";


CustomType::CustomType()
{
}

QStringList CustomType::getEnumeration()
{
    if (!this->restrictions.contains(RestrictionEnumeration))
    {
        return QStringList();
    }

    QString enumerationString = this->restrictions[RestrictionEnumeration];
    return enumerationString.split(";");
}

void CustomType::setEnumeration(const QStringList& enumeration)
{
    QString enumerationString = enumeration.join(";");
    this->restrictions[RestrictionEnumeration] = enumerationString;
}

void CustomType::setItemType(const QString& itemType)
{
    this->restrictions[RestrictionItemType] = itemType;
}
