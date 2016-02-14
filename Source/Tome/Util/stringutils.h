#ifndef STRINGUTILS
#define STRINGUTILS

#include <QString>

namespace Tome
{
    inline bool qStringLessThanLowerCase(const QString& e1, const QString& e2)
    {
        return e1.toLower() < e2.toLower();
    }
}

#endif // STRINGUTILS
