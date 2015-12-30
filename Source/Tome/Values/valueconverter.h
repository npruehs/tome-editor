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

            QString BoolToString(const bool b) const;
            QString FieldTypeToString(const FieldType::FieldType& fieldType) const;

            bool StringToBool(const QString& s) const;
            FieldType::FieldType StringToFieldType(const QString& s) const;
    };
}

#endif // VALUECONVERTER_H
