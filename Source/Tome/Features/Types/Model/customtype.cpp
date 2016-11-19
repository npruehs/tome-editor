#include "customtype.h"

using namespace Tome;


const QString CustomType::FacetBaseType = "BaseType";
const QString CustomType::FacetEnumeration = "Enumeration";
const QString CustomType::FacetItemType = "ItemType";
const QString CustomType::FacetKeyType = "KeyType";
const QString CustomType::FacetValueType = "ValueType";


CustomType::CustomType()
{
}

QString CustomType::getBaseType() const
{
    if (!this->isDerivedType())
    {
        return QString();
    }

    return this->fundamentalFacets[FacetBaseType].toString();
}

QStringList CustomType::getEnumeration() const
{
    if (!this->isEnumeration())
    {
        return QStringList();
    }

    QString enumerationString = this->fundamentalFacets[FacetEnumeration].toString();

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

    return this->fundamentalFacets[FacetItemType].toString();
}

QString CustomType::getKeyType() const
{
    if (!this->isMap())
    {
        return QString();
    }

    return this->fundamentalFacets[FacetKeyType].toString();
}

QString CustomType::getValueType() const
{
    if (!this->isMap())
    {
        return QString();
    }

    return this->fundamentalFacets[FacetValueType].toString();
}

bool CustomType::isDerivedType() const
{
    return this->fundamentalFacets.contains(FacetBaseType);
}

bool CustomType::isEnumeration() const
{
    return this->fundamentalFacets.contains(FacetEnumeration);
}

bool CustomType::isList() const
{
    return this->fundamentalFacets.contains(FacetItemType);
}

bool CustomType::isMap() const
{
    return this->fundamentalFacets.contains(FacetKeyType) &&
            this->fundamentalFacets.contains(FacetValueType);
}

void CustomType::setBaseType(const QString& baseType)
{
    this->fundamentalFacets[FacetBaseType] = baseType;
}

void CustomType::setEnumeration(const QStringList& enumeration)
{
    QString enumerationString = enumeration.join(";");
    this->fundamentalFacets[FacetEnumeration] = enumerationString;
}

void CustomType::setItemType(const QString& itemType)
{
    this->fundamentalFacets[FacetItemType] = itemType;
}

void CustomType::setKeyType(const QString& keyType)
{
    this->fundamentalFacets[FacetKeyType] = keyType;
}

void CustomType::setValueType(const QString& valueType)
{
    this->fundamentalFacets[FacetValueType] = valueType;
}
