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

    inline QString splitAtCapitalLetters(const QString& s)
    {
        QString result;

        for (int i = 0; i < s.length() - 1; ++i)
        {
            result = result.append(s[i]);

            if (s[i].isLower() && s[i + 1].isUpper())
            {
                result = result.append(" ");
            }
        }

        result = result.append(s[s.length() - 1]);

        return result;
    }

    inline QString stripWhitespaces(const QString& s)
    {
        return s.simplified().replace(" ", "");
    }
}

#endif // STRINGUTILS
