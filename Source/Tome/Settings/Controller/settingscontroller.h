#ifndef SETTINGSCONTROLLER_H
#define SETTINGSCONTROLLER_H

#include <QSettings>
#include <QStringList>

namespace Tome
{
    class SettingsController
    {
        public:
            SettingsController();
            ~SettingsController();

            void addRecentProject(const QString& path);
            const QStringList getRecentProjects() const;
            void removeRecentProject(const QString& path);
            void setRecentProjects(const QStringList& recentProjects);

        private:
            static const QString SettingsRecentProjects;
            static const QString SettingsPath;

            QSettings* settings;
    };
}

#endif // SETTINGSCONTROLLER_H
