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

class UserSettingsWindow : public QDialog
{
        Q_OBJECT

    public:
        explicit UserSettingsWindow(
                Tome::SettingsController& settingsController,
                QWidget *parent = 0);
        ~UserSettingsWindow();

        bool getRunIntegrityChecksOnSave();
        bool getShowDescriptionColumnInsteadOfFieldTooltips();
        bool getExpandRecordTreeOnRefresh();

    protected:
        void showEvent(QShowEvent * event);

    private:
        Ui::UserSettingsWindow *ui;

        Tome::SettingsController& settingsController;
};

#endif // USERSETTINGSWINDOW_H
