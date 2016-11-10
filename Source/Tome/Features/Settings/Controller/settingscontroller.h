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
            bool getExpandRecordTree() const;
            void removeRecentProject(const QString& path);
            void setRecentProjects(const QStringList& recentProjects);
            void setRunIntegrityChecksOnSave(bool runIntegrityChecksOnSave);
            void setShowDescriptionColumnInsteadOfFieldTooltips(bool showDescriptionColumnInsteadOfFieldTooltips);
            void setExpandRecordTree(bool expandRecordTree);

        private:
            static const QString SettingPath;
            static const QString SettingRecentProjects;
            static const QString SettingRunIntegrityChecksOnSave;
            static const QString SettingShowDescriptionColumnInsteadOfFieldTooltips;
            static const QString SettingExpandRecordTree;

            QSettings* settings;
    };
}

#endif // SETTINGSCONTROLLER_H
