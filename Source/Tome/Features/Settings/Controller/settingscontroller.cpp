#include "settingscontroller.h"

#include <QCoreApplication>

using namespace Tome;


const QString SettingsController::SettingPath = "path";
const QString SettingsController::SettingRecentProjects = "recentProjects";
const QString SettingsController::SettingRunIntegrityChecksOnLoad = "runIntegrityChecksOnLoad";
const QString SettingsController::SettingRunIntegrityChecksOnSave = "runIntegrityChecksOnSave";
const QString SettingsController::SettingShowComponentNamesInRecordTable = "showComponentNamesInRecordTable";
const QString SettingsController::SettingShowDescriptionColumnInsteadOfFieldTooltips = "showDetailsColumnInsteadOfFieldTooltips";
const QString SettingsController::SettingExpandRecordTreeOnRefresh = "expandRecordTreeOnRefresh";
const QString SettingsController::SettingLastProjectPath = "lastProjectPath";


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
    qInfo(qUtf8Printable(QString("Adding %1 to recent projects list.").arg(path)));

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

bool SettingsController::getShowComponentNamesInRecordTable() const
{
    return this->settings->value(SettingShowComponentNamesInRecordTable).toBool();
}

bool SettingsController::getShowDescriptionColumnInsteadOfFieldTooltips() const
{
    return this->settings->value(SettingShowDescriptionColumnInsteadOfFieldTooltips).toBool();
}

bool SettingsController::getExpandRecordTreeOnRefresh() const
{
    return this->settings->value(SettingExpandRecordTreeOnRefresh).toBool();
}

const QString SettingsController::getLastProjectPath() const
{
    return this->settings->value(SettingLastProjectPath).toString();
}

void SettingsController::removeRecentProject(const QString& path)
{
    qInfo(qUtf8Printable(QString("Removing %1 from recent projects list.").arg(path)));

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
    qInfo(qUtf8Printable(QString("Setting run integrity checks on load to %1.")
          .arg(runIntegrityChecksOnLoad ? "true" : "false")));
    this->settings->setValue(SettingRunIntegrityChecksOnLoad, runIntegrityChecksOnLoad);
}

void SettingsController::setRunIntegrityChecksOnSave(bool runIntegrityChecksOnSave)
{
    qInfo(qUtf8Printable(QString("Setting run integrity checks on save to %1.")
          .arg(runIntegrityChecksOnSave ? "true" : "false")));
    this->settings->setValue(SettingRunIntegrityChecksOnSave, runIntegrityChecksOnSave);
}

void SettingsController::setShowComponentNamesInRecordTable(bool showComponentNamesInRecordTable)
{
    qInfo(qUtf8Printable(QString("Setting show component names in record table to %1.")
          .arg(showComponentNamesInRecordTable ? "true" : "false")));
    this->settings->setValue(SettingShowComponentNamesInRecordTable, showComponentNamesInRecordTable);
}

void SettingsController::setShowDescriptionColumnInsteadOfFieldTooltips(bool showDescriptionColumnInsteadOfFieldTooltips)
{
    qInfo(qUtf8Printable(QString("Setting show description column in stead of field tooltips to %1.")
          .arg(showDescriptionColumnInsteadOfFieldTooltips ? "true" : "false")));
    this->settings->setValue(SettingShowDescriptionColumnInsteadOfFieldTooltips, showDescriptionColumnInsteadOfFieldTooltips);
}

void SettingsController::setExpandRecordTreeOnRefresh(bool expandRecordTreeOnRefresh)
{
    qInfo(qUtf8Printable(QString("Setting expand record tree on refresh to %1.")
          .arg(expandRecordTreeOnRefresh ? "true" : "false")));
    this->settings->setValue(SettingExpandRecordTreeOnRefresh, expandRecordTreeOnRefresh);
}

void SettingsController::setLastProjectPath( const QString &path )

{
    qInfo(qUtf8Printable(QString("Setting last project path to %1.")
          .arg(path)));
    this->settings->setValue(SettingLastProjectPath, path);
}
