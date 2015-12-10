#ifndef STRINGUTILS
#define STRINGUTILS

#include <QString>

namespace Tome
{
    /**
     * @brief equals Compares the two passed strings for equality.
     * @param first First string to compare.
     * @param second Second string to compare.
     * @return true, if both strings are equal, and false otherwise.
     */
    inline bool equals(const QString& first, const QString& second)
    {
        return first.compare(second) == 0;
    }
}

#endif // STRINGUTILS
