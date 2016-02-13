#ifndef CONVERSIONUTILS_H
#define CONVERSIONUTILS_H

#include <QString>


namespace Tome
{
    inline QString BoolToString(const bool b)
    {
        return b ? "True" : "False";
    }

    inline bool StringToBool(const QString& s)
    {
        return s == "True";
    }
}

#endif // CONVERSIONUTILS_H

