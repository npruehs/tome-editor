#ifndef TOMESETTINGS_H
#define TOMESETTINGS_H

#include <QSettings>
#include <QSharedPointer>
#include <QStringList>


class TomeSettings
{
    public:
        TomeSettings();

        void addRecentProject(const QString& path);
        QStringList getRecentProjects();
        void removeRecentProject(const QString& path);
        void setRecentProjects(QStringList recentProjects);

    private:
        static const QString SettingsRecentProjects;
        static const QString SettingsPath;

        QSharedPointer<QSettings> settings;
};

#endif // TOMESETTINGS_H
