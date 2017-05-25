#ifndef SETTINGSCONTROLLER_H
#define SETTINGSCONTROLLER_H

#include <QSettings>
#include <QStringList>

namespace Tome
{
    /**
     * @brief Controller for accessing user settings.
     */
    class SettingsController
    {
        public:
            /**
             * @brief Constructs a new controller for accessing user settings.
             */
            SettingsController();
            ~SettingsController();

            /**
             * @brief Adds the specified full project path to the list of recent projects.
             * @param path Path of the project to add.
             */
            void addRecentProject(const QString& path);

            /**
             * @brief Gets a list of full paths to recently opened projects.
             * @return List of full paths to recently opened projects.
             */
            const QStringList getRecentProjects() const;

            /**
             * @brief Gets whether to run all integrity checks after opening a project.
             * @return Whether to run all integrity checks after opening a project, or not.
             */
            bool getRunIntegrityChecksOnLoad() const;

            /**
             * @brief Gets whether to run all integrity checks after saving a project.
             * @return Whether to run all integrity checks after saving a project, or not.
             */
            bool getRunIntegrityChecksOnSave() const;

            /**
             * @brief Gets whether to show the names of the respective component in front of the field name in the record table.
             * @return Whether to show the names of the respective component in front of the field name in the record table, or not.
             */
            bool getShowComponentNamesInRecordTable() const;

            /**
             * @brief Gets whether to show an additional column with the description of all record fields in the record table.
             * @return true, if showing the description of all record fields in an additional column in the record table, and false if showing tooltips instead.
             */
            bool getShowDescriptionColumnInsteadOfFieldTooltips() const;

            /**
             * @brief Gets whether to expand all nodes of the record tree every time after it has changed.
             * @return Whether to expand all nodes of the record tree every time after it has changed, or not.
             */
            bool getExpandRecordTreeOnRefresh() const;

            /**
             * @brief Gets the full path to the most recently opened project.
             * @return Full path of the most recently opened project.
             */
            const QString getLastProjectPath() const;

            /**
             * @brief Removes the specified full project path from the list of recent projects.
             * @param path Path of the project to remove.
             */
            void removeRecentProject(const QString& path);

            /**
             * @brief Sets the list of full paths to recently opened projects.
             * @param recentProjects List of full paths to recently opened projects.
             */
            void setRecentProjects(const QStringList& recentProjects);

            /**
             * @brief Sets whether to run all integrity checks after opening a project, or not.
             * @param runIntegrityChecksOnLoad Whether to run all integrity checks after opening a project.
             */
            void setRunIntegrityChecksOnLoad(bool runIntegrityChecksOnLoad);

            /**
             * @brief Sets whether to run all integrity checks after saving a project, or not.
             * @param runIntegrityChecksOnSave Whether to run all integrity checks after saving a project.
             */
            void setRunIntegrityChecksOnSave(bool runIntegrityChecksOnSave);

            /**
             * @brief Sets whether to show the names of the respective component in front of the field name in the record table.
             * @param showComponentNamesInRecordTable Whether to show the names of the respective component in front of the field name in the record table.
             */
            void setShowComponentNamesInRecordTable(bool showComponentNamesInRecordTable);

            /**
             * @brief Sets whether to show an additional column with the description of all record fields in the record table, or to show tooltips instead.
             * @param showDescriptionColumnInsteadOfFieldTooltips Whether to show an additional column with the description of all record fields in the record table.
             */
            void setShowDescriptionColumnInsteadOfFieldTooltips(bool showDescriptionColumnInsteadOfFieldTooltips);

            /**
             * @brief Sets whether to expand all nodes of the record tree every time after it has changed, or not.
             * @param expandRecordTreeOnRefresh Whether to expand all nodes of the record tree every time after it has changed.
             */
            void setExpandRecordTreeOnRefresh(bool expandRecordTreeOnRefresh);

            /**
             * @brief Sets the full path to the most recently opened project.
             * @param path Full path to the most recently opened project.
             */
            void setLastProjectPath(const QString &path);

        private:
            static const QString SettingPath;
            static const QString SettingRecentProjects;
            static const QString SettingRunIntegrityChecksOnLoad;
            static const QString SettingRunIntegrityChecksOnSave;
            static const QString SettingShowComponentNamesInRecordTable;
            static const QString SettingShowDescriptionColumnInsteadOfFieldTooltips;
            static const QString SettingExpandRecordTreeOnRefresh;
            static const QString SettingLastProjectPath;

            QSettings* settings;
    };
}

#endif // SETTINGSCONTROLLER_H
