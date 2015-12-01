#ifndef VALUECONVERTER_H
#define VALUECONVERTER_H

#include <QString>

#include "../Fields/fieldtype.h"

namespace Tome
{
    class ValueConverter
    {
    public:
        ValueConverter();

        QString FieldTypeToString(FieldType::FieldType fieldType);
    };
}

#endif // VALUECONVERTER_H
