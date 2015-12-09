#ifndef PATHUTILS
#define PATHUTILS

#include <QDir>
#include <QString>

namespace Tome
{
    const QString FieldDefinitionFileExtension = ".tfields";
    const QString ProjectFileExtension = ".tproj";
    const QString RecordFileExtension = ".tdata";

    /**
     * @brief combinePaths Combines the paths specified by the two passes strings.
     * @param first First path to combine.
     * @param second Second path to combine.
     * @return Both paths, combined with the platform-specific path seperator.
     */
    QString combinePaths(const QString& first, const QString& second)
    {
        return QDir::cleanPath(first + QDir::separator() + second);
    }
}

#endif // PATHUTILS

