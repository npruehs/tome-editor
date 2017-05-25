#ifndef PATHUTILS_H
#define PATHUTILS_H

#include <QDesktopServices>
#include <QDir>
#include <QFileInfo>
#include <QString>
#include <QUrl>

namespace Tome
{
    /**
     * @brief Combines the paths specified by the two passes strings.
     * @param first First path to combine.
     * @param second Second path to combine.
     * @return Both paths, combined with the platform-specific path seperator.
     */
    inline QString combinePaths(const QString& first, const QString& second)
    {
        return QDir::cleanPath(first + QDir::separator() + second);
    }

    /**
     * @brief Opens the specified file with the default program registered in the underlying OS.
     * @param filePath File to open.
     */
    inline void openFileWithDefaultProgram(const QString& filePath)
    {
        const QFileInfo file(filePath);
        const QUrl url = QUrl::fromLocalFile(file.absoluteFilePath());
        QDesktopServices::openUrl(url);
    }

    /**
     * @brief Opens the Explorer (on Windows) or the Finder (on Mac OS) and navigates to the specified file.
     * @param filePath File to navigate to.
     */
    inline void showFileInExplorerOrFinder(const QString& filePath)
    {
        const QFileInfo file(filePath);
        const QUrl url = QUrl::fromLocalFile(file.absolutePath());
        QDesktopServices::openUrl(url);
    }
}

#endif // PATHUTILS_H

