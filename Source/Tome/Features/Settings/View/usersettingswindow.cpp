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

bool UserSettingsWindow::getRunIntegrityChecksOnSave()
{
    return this->ui->checkBoxRunIntegrityChecksOnSave->isChecked();
}

bool UserSettingsWindow::getShowDescriptionColumnInsteadOfFieldTooltips()
{
    return this->ui->checkBoxShowDescriptionColumnInsteadOfFieldTooltips->isChecked();
}

bool UserSettingsWindow::getExpandRecordTree()
{
    return this->ui->checkBoxExpandRecordTree->isChecked();
}

void UserSettingsWindow::showEvent(QShowEvent* event)
{
    Q_UNUSED(event);

    // Update view from model.
    bool runIntegrityChecksOnSave = this->settingsController.getRunIntegrityChecksOnSave();
    this->ui->checkBoxRunIntegrityChecksOnSave->setChecked(runIntegrityChecksOnSave);

    bool showDescriptionColumnInsteadOfFieldTooltips = this->settingsController.getShowDescriptionColumnInsteadOfFieldTooltips();
    this->ui->checkBoxShowDescriptionColumnInsteadOfFieldTooltips->setChecked(showDescriptionColumnInsteadOfFieldTooltips);

    bool showExpandRecordTree = this->settingsController.getExpandRecordTree();
    this->ui->checkBoxExpandRecordTree->setChecked(showExpandRecordTree);
}
