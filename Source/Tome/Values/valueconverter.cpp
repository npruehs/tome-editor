#include "valueconverter.h"

using namespace Tome;


ValueConverter::ValueConverter()
{
}

QString ValueConverter::BoolToString(const bool b) const
{
    return b ? "True" : "False";
}

bool ValueConverter::StringToBool(const QString& s) const
{
    return s == "True";
}
