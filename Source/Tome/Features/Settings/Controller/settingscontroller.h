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
            bool getRunIntegrityChecksOnLoad() const;
            bool getRunIntegrityChecksOnSave() const;
            bool getShowComponentNamesInRecordTable() const;
            bool getShowDescriptionColumnInsteadOfFieldTooltips() const;
            bool getExpandRecordTreeOnRefresh() const;
            void removeRecentProject(const QString& path);
            void setRecentProjects(const QStringList& recentProjects);
            void setRunIntegrityChecksOnLoad(bool runIntegrityChecksOnLoad);
            void setRunIntegrityChecksOnSave(bool runIntegrityChecksOnSave);
            void setShowComponentNamesInRecordTable(bool showComponentNamesInRecordTable);
            void setShowDescriptionColumnInsteadOfFieldTooltips(bool showDescriptionColumnInsteadOfFieldTooltips);
            void setExpandRecordTreeOnRefresh(bool expandRecordTreeOnRefresh);

        private:
            static const QString SettingPath;
            static const QString SettingRecentProjects;
            static const QString SettingRunIntegrityChecksOnLoad;
            static const QString SettingRunIntegrityChecksOnSave;
            static const QString SettingShowComponentNamesInRecordTable;
            static const QString SettingShowDescriptionColumnInsteadOfFieldTooltips;
            static const QString SettingExpandRecordTreeOnRefresh;

            QSettings* settings;
    };
}

#endif // SETTINGSCONTROLLER_H
