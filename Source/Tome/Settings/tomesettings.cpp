#include "tomesettings.h"

#include <QCoreApplication>


const QString TomeSettings::SettingsRecentProjects = "recentProjects";
const QString TomeSettings::SettingsPath = "path";


TomeSettings::TomeSettings()
{
    QSettings* s = new QSettings(
                QSettings::IniFormat,
                QSettings::UserScope,
                QCoreApplication::organizationName(),
                QCoreApplication::applicationName());

    this->settings = QSharedPointer<QSettings>(s);
}

void TomeSettings::addRecentProject(const QString& path)
{
    QStringList recentProjects = this->getRecentProjects();

    if (recentProjects.contains(path))
    {
        return;
    }

    recentProjects.push_front(path);

    this->setRecentProjects(recentProjects);
}

QStringList TomeSettings::getRecentProjects()
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

void TomeSettings::removeRecentProject(const QString& path)
{
    QStringList recentProjects = this->getRecentProjects();
    recentProjects.removeOne(path);
    this->setRecentProjects(recentProjects);
}

void TomeSettings::setRecentProjects(QStringList recentProjects)
{
    this->settings->beginWriteArray(SettingsRecentProjects);
    for (int i = 0; i < recentProjects.size(); ++i)
    {
        this->settings->setArrayIndex(i);
        this->settings->setValue(SettingsPath, recentProjects.at(i));
    }
    this->settings->endArray();
}
