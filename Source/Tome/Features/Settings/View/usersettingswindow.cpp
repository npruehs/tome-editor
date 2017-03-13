#include "usersettingswindow.h"
#include "ui_usersettingswindow.h"

#include "../Controller/settingscontroller.h"

using namespace Tome;

UserSettingsWindow::UserSettingsWindow(
        SettingsController& settingsController,
        QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserSettingsWindow),
    settingsController(settingsController)
{
    ui->setupUi(this);
}

UserSettingsWindow::~UserSettingsWindow()
{
    delete ui;
}

bool UserSettingsWindow::getRunIntegrityChecksOnLoad()
{
    return this->ui->checkBoxRunIntegrityChecksOnLoad->isChecked();
}

bool UserSettingsWindow::getRunIntegrityChecksOnSave()
{
    return this->ui->checkBoxRunIntegrityChecksOnSave->isChecked();
}

bool UserSettingsWindow::getShowComponentNamesInRecordTable()
{
    return this->ui->checkBoxShowComponentNamesInRecordTable->isChecked();
}

bool UserSettingsWindow::getShowDescriptionColumnInsteadOfFieldTooltips()
{
    return this->ui->checkBoxShowDescriptionColumnInsteadOfFieldTooltips->isChecked();
}

bool UserSettingsWindow::getExpandRecordTreeOnRefresh()
{
    return this->ui->checkBoxExpandRecordTreeOnRefresh->isChecked();
}

void UserSettingsWindow::showEvent(QShowEvent* event)
{
    Q_UNUSED(event)

    // Update view from model.
    bool runIntegrityChecksOnLoad = this->settingsController.getRunIntegrityChecksOnLoad();
    this->ui->checkBoxRunIntegrityChecksOnLoad->setChecked(runIntegrityChecksOnLoad);

    bool runIntegrityChecksOnSave = this->settingsController.getRunIntegrityChecksOnSave();
    this->ui->checkBoxRunIntegrityChecksOnSave->setChecked(runIntegrityChecksOnSave);

    bool showComponentNamesInRecordTable = this->settingsController.getShowComponentNamesInRecordTable();
    this->ui->checkBoxShowComponentNamesInRecordTable->setChecked(showComponentNamesInRecordTable);

    bool showDescriptionColumnInsteadOfFieldTooltips = this->settingsController.getShowDescriptionColumnInsteadOfFieldTooltips();
    this->ui->checkBoxShowDescriptionColumnInsteadOfFieldTooltips->setChecked(showDescriptionColumnInsteadOfFieldTooltips);

    bool expandRecordTreeOnRefresh = this->settingsController.getExpandRecordTreeOnRefresh();
    this->ui->checkBoxExpandRecordTreeOnRefresh->setChecked(expandRecordTreeOnRefresh);
}
