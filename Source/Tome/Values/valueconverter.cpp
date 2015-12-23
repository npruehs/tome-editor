#include "valueconverter.h"

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
    if (s == "Boolean")
    {
        return FieldType::Boolean;
    }
    if (s == "Color")
    {
        return FieldType::Color;
    }
    if (s == "Float")
    {
        return FieldType::Float;
    }
    if (s == "Int")
    {
        return FieldType::Int;
    }
    if (s == "Reference")
    {
        return FieldType::Reference;
    }
    if (s == "String")
    {
        return FieldType::String;
    }
    if (s == "Vector3F")
    {
        return FieldType::Vector3F;
    }

    return FieldType::None;
}
