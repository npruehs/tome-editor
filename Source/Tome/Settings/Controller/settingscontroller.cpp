#include "settingscontroller.h"

#include <QCoreApplication>

using namespace Tome;


const QString SettingsController::SettingsRecentProjects = "recentProjects";
const QString SettingsController::SettingsPath = "path";


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

    if (recentProjects.contains(path))
    {
        return;
    }

    recentProjects.push_front(path);

    this->setRecentProjects(recentProjects);
}

const QStringList SettingsController::getRecentProjects() const
{
    QStringList recentProjects;

    int size = this->settings->beginReadArray(SettingsRecentProjects);
    for (int i = 0; i < size; ++i)
    {
        this->settings->setArrayIndex(i);
        QString path = this->settings->value(SettingsPath).toString();
        recentProjects.push_back(path);
    }
    this->settings->endArray();

    return recentProjects;
}

void SettingsController::removeRecentProject(const QString& path)
{
    QStringList recentProjects = this->getRecentProjects();
    recentProjects.removeOne(path);
    this->setRecentProjects(recentProjects);
}

void SettingsController::setRecentProjects(const QStringList& recentProjects)
{
    this->settings->beginWriteArray(SettingsRecentProjects);
    for (int i = 0; i < recentProjects.size(); ++i)
    {
        this->settings->setArrayIndex(i);
        this->settings->setValue(SettingsPath, recentProjects.at(i));
    }
    this->settings->endArray();
}
