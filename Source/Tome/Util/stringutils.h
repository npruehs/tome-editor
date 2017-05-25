#ifndef STRINGUTILS
#define STRINGUTILS

#include <QString>

namespace Tome
{
    /**
     * @brief Checks whether the passed string contains any spaces, tabs, carriage returns or line feeds.
     * @param s String to check.
     * @return true, if the string contains any whitespaces, and false otherwise.
     */
    inline bool containsWhitespaces(const QString& s)
    {
        return s.contains(" ") ||
               s.contains("\t") ||
               s.contains("\r") ||
               s.contains("\n");
    }

    /**
     * @brief Converts both passed strings to lower case and compares them.
     * @param lhs First string to compare.
     * @param rhs Second string to compare.
     * @return true, if the first string is less than the second string, with respect to alphanumerical order.
     */
    inline bool qStringLessThanLowerCase(const QString& lhs, const QString& rhs)
    {
        return lhs.toLower() < rhs.toLower();
    }

    /**
     * @brief Inserts whitespaces before each capital letter except for the first one.
     * @param s String to insert whitespaces into.
     * @return Passed string with whitespaces before each capital letter except for the first one.
     */
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

    /**
     * @brief Removes all whitespaces from the specified string.
     * @param s String to remove all whitespaces from.
     * @return Passed string without any whitespaces.
     */
    inline QString stripWhitespaces(const QString& s)
    {
        return s.simplified().replace(" ", "");
    }
}

#endif // STRINGUTILS
