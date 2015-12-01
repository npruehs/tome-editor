#ifndef PATHUTILS
#define PATHUTILS

#include <QDir>
#include <QString>

namespace Tome
{
    QString combinePaths(const QString& first, const QString& second)
    {
        return QDir::cleanPath(first + QDir::separator() + second);
    }
}

#endif // PATHUTILS

