#include "customtype.h"

using namespace Tome;


const QString CustomType::RestrictionEnumeration = "Enumeration";
const QString CustomType::RestrictionItemType = "ItemType";


CustomType::CustomType()
{
}

QStringList CustomType::getEnumeration() const
{
    if (!this->isEnumeration())
    {
        return QStringList();
    }

    QString enumerationString = this->restrictions[RestrictionEnumeration];
    return enumerationString.split(";");
}

QString CustomType::getItemType() const
{
    if (!this->isList())
    {
        return QString();
    }

    return this->restrictions[RestrictionItemType];
}

bool CustomType::isEnumeration() const
{
    return this->restrictions.contains(RestrictionEnumeration);
}

bool CustomType::isList() const
{
    return this->restrictions.contains(RestrictionItemType);
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
