#include "valueconverter.h"

using namespace Tome;


ValueConverter::ValueConverter()
{
}

QString ValueConverter::FieldTypeToString(const FieldType::FieldType& fieldType) const
{
    switch (fieldType)
    {
        case FieldType::None:
            return "None";

        case FieldType::Integer:
            return "Integer";

        case FieldType::Real:
            return "Real";

        case FieldType::String:
            return "String";
    }

    return "Invalid";
}

FieldType::FieldType ValueConverter::StringToFieldType(const QString& s) const
{
    if (s == "Integer")
    {
        return FieldType::Integer;
    }
    if (s == "Real")
    {
        return FieldType::Real;
    }
    if (s == "String")
    {
        return FieldType::String;
    }

    return FieldType::None;
}
