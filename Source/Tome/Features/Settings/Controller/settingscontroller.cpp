#include "settingscontroller.h"

#include <QCoreApplication>

using namespace Tome;


const QString SettingsController::SettingPath = "path";
const QString SettingsController::SettingRecentProjects = "recentProjects";
const QString SettingsController::SettingRunIntegrityChecksOnSave = "runIntegrityChecksOnSave";
const QString SettingsController::SettingShowDescriptionColumnInsteadOfFieldTooltips = "showDetailsColumnInsteadOfFieldTooltips";


SettingsController::SettingsController()
{
    this->settings = new QSettings(
                QSettings::IniFormat,
                QSettings::UserScope,
                QCoreApplication::organizationName(),
                QCoreApplication::applicationName());
}

SettingsController::~SettingsController()
{
    delete this->settings;
}

void SettingsController::addRecentProject(const QString& path)
{
    QStringList recentProjects = this->getRecentProjects();

    recentProjects.removeOne(path);
    recentProjects.push_front(path);

    this->setRecentProjects(recentProjects);
}

const QStringList SettingsController::getRecentProjects() const
{
    QStringList recentProjects;

    int size = this->settings->beginReadArray(SettingRecentProjects);
    for (int i = 0; i < size; ++i)
    {
        this->settings->setArrayIndex(i);
        QString path = this->settings->value(SettingPath).toString();
        recentProjects.push_back(path);
    }
    this->settings->endArray();

    return recentProjects;
}

bool SettingsController::getRunIntegrityChecksOnSave() const
{
    return this->settings->value(SettingRunIntegrityChecksOnSave).toBool();
}

bool SettingsController::getShowDescriptionColumnInsteadOfFieldTooltips() const
{
    return this->settings->value(SettingShowDescriptionColumnInsteadOfFieldTooltips).toBool();
}

void SettingsController::removeRecentProject(const QString& path)
{
    QStringList recentProjects = this->getRecentProjects();
    recentProjects.removeOne(path);
    this->setRecentProjects(recentProjects);
}

void SettingsController::setRecentProjects(const QStringList& recentProjects)
{
    this->settings->beginWriteArray(SettingRecentProjects);
    for (int i = 0; i < recentProjects.size(); ++i)
    {
        this->settings->setArrayIndex(i);
        this->settings->setValue(SettingPath, recentProjects.at(i));
    }
    this->settings->endArray();
}

void SettingsController::setRunIntegrityChecksOnSave(bool runIntegrityChecksOnSave)
{
    this->settings->setValue(SettingRunIntegrityChecksOnSave, runIntegrityChecksOnSave);
}

void SettingsController::setShowDescriptionColumnInsteadOfFieldTooltips(bool showDescriptionColumnInsteadOfFieldTooltips)
{
    this->settings->setValue(SettingShowDescriptionColumnInsteadOfFieldTooltips, showDescriptionColumnInsteadOfFieldTooltips);
}
