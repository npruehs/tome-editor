#ifndef USERSETTINGSWINDOW_H
#define USERSETTINGSWINDOW_H

#include <QDialog>

namespace Ui {
    class UserSettingsWindow;
}

namespace Tome
{
    class SettingsController;
}

/**
 * @brief Window for editing user-specific application settings.
 */
class UserSettingsWindow : public QDialog
{
        Q_OBJECT

    public:
        /**
         * @brief Constructs a new window for editing user-specific application settings.
         * @param settingsController Controller for accessing user settings.
         * @param parent Optional owner widget.
         */
        explicit UserSettingsWindow(
                Tome::SettingsController& settingsController,
                QWidget *parent = 0);
        ~UserSettingsWindow();

        /**
         * @brief Gets whether to run all integrity checks after opening a project.
         * @return Whether to run all integrity checks after opening a project, or not.
         */
        bool getRunIntegrityChecksOnLoad();

        /**
         * @brief Gets whether to run all integrity checks after saving a project.
         * @return Whether to run all integrity checks after saving a project, or not.
         */
        bool getRunIntegrityChecksOnSave();

        /**
         * @brief Gets whether to show the names of the respective component in front of the field name in the record table.
         * @return Whether to show the names of the respective component in front of the field name in the record table, or not.
         */
        bool getShowComponentNamesInRecordTable();

        /**
         * @brief Gets whether to show an additional column with the description of all record fields in the record table.
         * @return true, if showing the description of all record fields in an additional column in the record table, and false if showing tooltips instead.
         */
        bool getShowDescriptionColumnInsteadOfFieldTooltips();

        /**
         * @brief Gets whether to expand all nodes of the record tree every time after it has changed.
         * @return Whether to expand all nodes of the record tree every time after it has changed, or not.
         */
        bool getExpandRecordTreeOnRefresh();

    protected:
        /**
         * @brief Sets up this window, updating the view with the stored settings.
         * @param event Event for showing this window.
         */
        void showEvent(QShowEvent * event);

    private:
        Ui::UserSettingsWindow *ui;

        Tome::SettingsController& settingsController;
};

#endif // USERSETTINGSWINDOW_H
