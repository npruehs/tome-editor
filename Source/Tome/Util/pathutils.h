#ifndef PATHUTILS_H
#define PATHUTILS_H

#include <QDir>
#include <QString>

namespace Tome
{
    /**
     * @brief combinePaths Combines the paths specified by the two passes strings.
     * @param first First path to combine.
     * @param second Second path to combine.
     * @return Both paths, combined with the platform-specific path seperator.
     */
    inline QString combinePaths(const QString& first, const QString& second)
    {
        return QDir::cleanPath(first + QDir::separator() + second);
    }
}

#endif // PATHUTILS_H

