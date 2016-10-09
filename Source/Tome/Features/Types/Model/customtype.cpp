#include "customtype.h"

using namespace Tome;


const QString CustomType::RestrictionEnumeration = "Enumeration";
const QString CustomType::RestrictionItemType = "ItemType";
const QString CustomType::RestrictionKeyType = "KeyType";
const QString CustomType::RestrictionValueType = "ValueType";


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

    if (enumerationString.isEmpty())
    {
        // http://doc.qt.io/qt-5/qstring.html#split
        // If "sep" does not match anywhere in the string, split() returns a
        // single-element list containing this string.
        // If the enum contains exactly one value, that's fine.
        // If it doesn't contain any values, we don't want a single empty
        // enumeration member here.
        return QStringList();
    }

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

QString CustomType::getKeyType() const
{
    if (!this->isMap())
    {
        return QString();
    }

    return this->restrictions[RestrictionKeyType];
}

QString CustomType::getValueType() const
{
    if (!this->isMap())
    {
        return QString();
    }

    return this->restrictions[RestrictionValueType];
}

bool CustomType::isEnumeration() const
{
    return this->restrictions.contains(RestrictionEnumeration);
}

bool CustomType::isList() const
{
    return this->restrictions.contains(RestrictionItemType);
}

bool CustomType::isMap() const
{
    return this->restrictions.contains(RestrictionKeyType) &&
            this->restrictions.contains(RestrictionValueType);
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

void CustomType::setKeyType(const QString& keyType)
{
    this->restrictions[RestrictionKeyType] = keyType;
}

void CustomType::setValueType(const QString& valueType)
{
    this->restrictions[RestrictionValueType] = valueType;
}
