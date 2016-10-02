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
            bool getRunIntegrityChecksOnSave() const;
            bool getShowDescriptionColumnInsteadOfFieldTooltips() const;
            void removeRecentProject(const QString& path);
            void setRecentProjects(const QStringList& recentProjects);
            void setRunIntegrityChecksOnSave(bool runIntegrityChecksOnSave);
            void setShowDescriptionColumnInsteadOfFieldTooltips(bool showDescriptionColumnInsteadOfFieldTooltips);

        private:
            static const QString SettingPath;
            static const QString SettingRecentProjects;
            static const QString SettingRunIntegrityChecksOnSave;
            static const QString SettingShowDescriptionColumnInsteadOfFieldTooltips;

            QSettings* settings;
    };
}

#endif // SETTINGSCONTROLLER_H
