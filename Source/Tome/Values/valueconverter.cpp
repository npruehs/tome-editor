#include "valueconverter.h"

#include "../Util/stringutils.h"

using namespace Tome;


ValueConverter::ValueConverter()
{
}

QString ValueConverter::FieldTypeToString(const FieldType::FieldType& fieldType) const
{
    switch (fieldType)
    {
        case FieldType::Boolean:
            return "Boolean";

        case FieldType::Color:
            return "Color";

        case FieldType::Float:
            return "Float";

        case FieldType::Int:
            return "Int";

        case FieldType::None:
            return "None";

        case FieldType::Reference:
            return "Reference";

        case FieldType::String:
            return "String";

        case FieldType::Vector3F:
            return "Vector3F";
    }

    return "Invalid";
}

FieldType::FieldType ValueConverter::StringToFieldType(const QString& s) const
{
    if (equals(s, "Boolean"))
    {
        return FieldType::Boolean;
    }
    if (equals(s, "Color"))
    {
        return FieldType::Color;
    }
    if (equals(s, "Float"))
    {
        return FieldType::Float;
    }
    if (equals(s, "Int"))
    {
        return FieldType::Int;
    }
    if (equals(s, "Reference"))
    {
        return FieldType::Reference;
    }
    if (equals(s, "String"))
    {
        return FieldType::String;
    }
    if (equals(s, "Vector3F"))
    {
        return FieldType::Vector3F;
    }

    return FieldType::None;
}
