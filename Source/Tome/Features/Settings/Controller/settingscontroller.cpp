#include "settingscontroller.h"

#include <QCoreApplication>

using namespace Tome;


const QString SettingsController::SettingPath = "path";
const QString SettingsController::SettingRecentProjects = "recentProjects";
const QString SettingsController::SettingRunIntegrityChecksOnLoad = "runIntegrityChecksOnLoad";
const QString SettingsController::SettingRunIntegrityChecksOnSave = "runIntegrityChecksOnSave";
const QString SettingsController::SettingShowDescriptionColumnInsteadOfFieldTooltips = "showDetailsColumnInsteadOfFieldTooltips";
const QString SettingsController::SettingExpandRecordTreeOnRefresh = "expandRecordTreeOnRefresh";


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
    qInfo(QString("Adding %1 to recent projects list.").arg(path).toUtf8().constData());

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

bool SettingsController::getRunIntegrityChecksOnLoad() const
{
    return this->settings->value(SettingRunIntegrityChecksOnLoad).toBool();
}

bool SettingsController::getRunIntegrityChecksOnSave() const
{
    return this->settings->value(SettingRunIntegrityChecksOnSave).toBool();
}

bool SettingsController::getShowDescriptionColumnInsteadOfFieldTooltips() const
{
    return this->settings->value(SettingShowDescriptionColumnInsteadOfFieldTooltips).toBool();
}

bool SettingsController::getExpandRecordTreeOnRefresh() const
{
    return this->settings->value(SettingExpandRecordTreeOnRefresh).toBool();
}

void SettingsController::removeRecentProject(const QString& path)
{
    qInfo(QString("Removing %1 from recent projects list.").arg(path).toUtf8().constData());

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

void SettingsController::setRunIntegrityChecksOnLoad(bool runIntegrityChecksOnLoad)
{
    qInfo(QString("Setting run integrity checks on load to %1.")
          .arg(runIntegrityChecksOnLoad ? "true" : "false")
          .toUtf8().constData());
    this->settings->setValue(SettingRunIntegrityChecksOnLoad, runIntegrityChecksOnLoad);
}

void SettingsController::setRunIntegrityChecksOnSave(bool runIntegrityChecksOnSave)
{
    qInfo(QString("Setting run integrity checks on save to %1.")
          .arg(runIntegrityChecksOnSave ? "true" : "false")
          .toUtf8().constData());
    this->settings->setValue(SettingRunIntegrityChecksOnSave, runIntegrityChecksOnSave);
}

void SettingsController::setShowDescriptionColumnInsteadOfFieldTooltips(bool showDescriptionColumnInsteadOfFieldTooltips)
{
    qInfo(QString("Setting show description column in stead of field tooltips to %1.")
          .arg(showDescriptionColumnInsteadOfFieldTooltips ? "true" : "false")
          .toUtf8().constData());
    this->settings->setValue(SettingShowDescriptionColumnInsteadOfFieldTooltips, showDescriptionColumnInsteadOfFieldTooltips);
}

void SettingsController::setExpandRecordTreeOnRefresh(bool expandRecordTreeOnRefresh)
{
    qInfo(QString("Setting expand record tree on refresh to %1.")
          .arg(expandRecordTreeOnRefresh ? "true" : "false")
          .toUtf8().constData());
    this->settings->setValue(SettingExpandRecordTreeOnRefresh, expandRecordTreeOnRefresh);
}

