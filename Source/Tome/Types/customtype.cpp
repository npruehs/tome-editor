#include "customtype.h"

using namespace Tome;


const QString CustomType::RestrictionEnumeration = "Enumeration";


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

void CustomType::setEnumeration(QStringList enumeration)
{
    QString enumerationString = enumeration.join(";");
    this->restrictions[RestrictionEnumeration] = enumerationString;
}
