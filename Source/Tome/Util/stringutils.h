#ifndef STRINGUTILS
#define STRINGUTILS

#include <QString>

namespace Tome
{
    inline bool containsWhitespaces(const QString& s)
    {
        return s.simplified().contains(" ");
    }

    inline bool qStringLessThanLowerCase(const QString& e1, const QString& e2)
    {
        return e1.toLower() < e2.toLower();
    }

    inline QString stripWhitespaces(const QString& s)
    {
        return s.simplified().replace(" ", "");
    }
}

#endif // STRINGUTILS
