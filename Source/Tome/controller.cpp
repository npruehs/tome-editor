#include "controller.h"

#include <stdexcept>

#include <QFile>
#include <QFileInfo>
#include <QObject>
#include <QTextStream>

#include "Components/Controller/componentscontroller.h"
#include "Export/Controller/exportcontroller.h"
#include "Fields/Controller/fielddefinitionscontroller.h"
#include "Fields/Controller/fielddefinitionsetserializer.h"
#include "Projects/Controller/projectserializer.h"
#include "Records/Controller/recordscontroller.h"
#include "Records/Controller/recordsetserializer.h"
#include "Settings/Controller/settingscontroller.h"
#include "Types/Controller/typescontroller.h"
#include "Util/pathutils.h"

using namespace Tome;


const QString Controller::FieldDefinitionFileExtension = ".tfields";
const QString Controller::ProjectFileExtension = ".tproj";
const QString Controller::RecordFileExtension = ".tdata";
const QString Controller::RecordExportComponentTemplateExtension = ".texportc";
const QString Controller::RecordExportComponentDelimiterExtension = ".texportcd";
const QString Controller::RecordExportRecordFileTemplateExtension = ".texportf";
const QString Controller::RecordExportRecordTemplateExtension = ".texportr";
const QString Controller::RecordExportRecordDelimiterExtension = ".texportrd";
const QString Controller::RecordExportFieldValueTemplateExtension = ".texportv";
const QString Controller::RecordExportFieldValueDelimiterExtension = ".texportvd";


Controller::Controller() :
    componentsController(new ComponentsController()),
    fieldDefinitionsController(new FieldDefinitionsController()),
    recordsController(new RecordsController(*this->fieldDefinitionsController)),
    exportController(new ExportController(*this->fieldDefinitionsController, *this->recordsController)),
    settingsController(new SettingsController()),
    typesController(new TypesController())
{
}

Controller::~Controller()
{
    delete this->componentsController;
    delete this->fieldDefinitionsController;
    delete this->recordsController;
    delete this->exportController;
    delete this->settingsController;
    delete this->typesController;
}

ComponentsController& Controller::getComponentsController()
{
    return *this->componentsController;
}

FieldDefinitionsController& Controller::getFieldDefinitionsController()
{
    return *this->fieldDefinitionsController;
}

RecordsController& Controller::getRecordsController()
{
    return *this->recordsController;
}

ExportController& Controller::getExportController()
{
    return *this->exportController;
}

SettingsController& Controller::getSettingsController()
{
    return *this->settingsController;
}

TypesController& Controller::getTypesController()
{
    return *this->typesController;
}

void Controller::createProject(const QString& projectName, const QString& projectPath)
{
    // Create new project.
    QSharedPointer<Project> newProject = QSharedPointer<Project>::create();
    newProject->name = projectName;
    newProject->path = projectPath;

    // Create field definition set.
    FieldDefinitionSet fieldDefinitionSet = FieldDefinitionSet();
    fieldDefinitionSet.name = projectName;
    newProject->fieldDefinitionSets.push_back(fieldDefinitionSet);

    // Create record set.
    RecordSet recordSet = RecordSet();
    recordSet.name = projectName;
    newProject->recordSets.push_back(recordSet);

    // Write project files.
    this->saveProject(newProject);

    // Set project reference.
    this->setProject(newProject);
}

const QString Controller::getProjectName() const
{
    return this->project->name;
}

const QString Controller::getProjectPath() const
{
    return this->project->path;
}

bool Controller::isProjectLoaded() const
{
    return this->project != 0;
}

void Controller::openProject(const QString& projectFileName)
{
    if (projectFileName.count() <= 0)
    {
        return;
    }

    // Open project file.
    QFile projectFile(projectFileName);
    QFileInfo projectFileInfo(projectFileName);

    const QString projectPath = projectFileInfo.path();

    if (projectFile.open(QIODevice::ReadOnly))
    {
        // Load project from file.
        ProjectSerializer projectSerializer = ProjectSerializer();
        QSharedPointer<Project> project = QSharedPointer<Project>::create();
        project->path = projectPath;

        try
        {
            projectSerializer.deserialize(projectFile, project);
        }
        catch (const std::runtime_error& e)
        {
            QString errorMessage = QObject::tr("File could not be read: ") + projectFileName + "\r\n" + e.what();
            throw std::runtime_error(errorMessage.toStdString());
        }

        // Load field definition files.
        FieldDefinitionSetSerializer fieldDefinitionSerializer = FieldDefinitionSetSerializer();

        for (int i = 0; i < project->fieldDefinitionSets.size(); ++i)
        {
            FieldDefinitionSet& fieldDefinitionSet = project->fieldDefinitionSets[i];

            // Open field definition file.
            const QString fullFieldDefinitionSetPath = combinePaths(projectPath, fieldDefinitionSet.name + FieldDefinitionFileExtension);
            QFile fieldDefinitionFile(fullFieldDefinitionSetPath);

            if (fieldDefinitionFile.open(QIODevice::ReadOnly))
            {
                try
                {
                    fieldDefinitionSerializer.deserialize(fieldDefinitionFile, fieldDefinitionSet);
                }
                catch (const std::runtime_error& e)
                {
                    QString errorMessage = QObject::tr("File could not be read: ") + fullFieldDefinitionSetPath + "\r\n" + e.what();
                    throw std::runtime_error(errorMessage.toStdString());
                }
            }
            else
            {
                QString errorMessage = QObject::tr("File could not be read:\r\n") + fullFieldDefinitionSetPath;
                throw std::runtime_error(errorMessage.toStdString());
            }
        }

        // Load record files.
        RecordSetSerializer recordSetSerializer = RecordSetSerializer();

        for (int i = 0; i < project->recordSets.size(); ++i)
        {
            RecordSet& recordSet = project->recordSets[i];

            // Open record file.
            const QString fullRecordSetPath = combinePaths(projectPath, recordSet.name + RecordFileExtension);
            QFile recordFile(fullRecordSetPath);

            if (recordFile.open(QIODevice::ReadOnly))
            {
                try
                {
                    recordSetSerializer.deserialize(recordFile, recordSet);
                }
                catch (const std::runtime_error& e)
                {
                    QString errorMessage = QObject::tr("File could not be read: ") + fullRecordSetPath + "\r\n" + e.what();
                    throw std::runtime_error(errorMessage.toStdString());
                }
            }
            else
            {
                QString errorMessage = QObject::tr("File could not be read:\r\n") + fullRecordSetPath;
                throw std::runtime_error(errorMessage.toStdString());
            }
        }

        // Load record export template files.
        for (RecordExportTemplateMap::iterator it = project->recordExportTemplates.begin();
             it != project->recordExportTemplates.end();
             ++it)
        {
            RecordExportTemplate& exportTemplate = it.value();

            try
            {
                exportTemplate.fieldValueDelimiter =
                        this->readFile(projectPath, exportTemplate.name + RecordExportFieldValueDelimiterExtension);
                exportTemplate.fieldValueTemplate =
                        this->readFile(projectPath, exportTemplate.name + RecordExportFieldValueTemplateExtension);
                exportTemplate.recordDelimiter =
                        this->readFile(projectPath, exportTemplate.name + RecordExportRecordDelimiterExtension);
                exportTemplate.recordFileTemplate =
                        this->readFile(projectPath, exportTemplate.name + RecordExportRecordFileTemplateExtension);
                exportTemplate.recordTemplate =
                        this->readFile(projectPath, exportTemplate.name + RecordExportRecordTemplateExtension);
                exportTemplate.componentDelimiter =
                        this->readFile(projectPath, exportTemplate.name + RecordExportComponentDelimiterExtension);
                exportTemplate.componentTemplate =
                        this->readFile(projectPath, exportTemplate.name + RecordExportComponentTemplateExtension);
            }
            catch (const std::runtime_error& e)
            {
                QString errorMessage = QObject::tr("File could not be read:\r\n") + e.what();
                throw std::runtime_error(errorMessage.toStdString());
            }
        }

        // Add to recent projects.
        this->settingsController->addRecentProject(projectFileName);

        // Set project reference.
        this->setProject(project);
    }
    else
    {
        // Remove from recent projects.
        this->settingsController->removeRecentProject(projectFileName);

        // Throw exception.
        QString errorMessage = QObject::tr("File could not be read:\r\n") + projectFileName;
        throw std::runtime_error(errorMessage.toStdString());
    }
}

void Controller::saveProject()
{
    this->saveProject(this->project);
}

void Controller::saveProject(QSharedPointer<Project> project)
{
    QString& projectPath = project->path;
    ProjectSerializer projectSerializer = ProjectSerializer();

    // Build file name.
    const QString fullProjectPath = this->getFullProjectPath(project);

    // Write project file.
    QFile projectFile(fullProjectPath);

    if (projectFile.open(QIODevice::ReadWrite | QIODevice::Truncate))
    {
        projectSerializer.serialize(projectFile, project);
    }
    else
    {
        QString errorMessage = QObject::tr("Destination file could not be written:\r\n") + fullProjectPath;
        throw std::runtime_error(errorMessage.toStdString());
    }

    // Write field definition sets.
    FieldDefinitionSetSerializer fieldDefinitionSetSerializer = FieldDefinitionSetSerializer();

    for (int i = 0; i < project->fieldDefinitionSets.size(); ++i)
    {
        const FieldDefinitionSet& fieldDefinitionSet = project->fieldDefinitionSets[i];

        // Build file name.
        const QString fieldDefinitionSetFileName = fieldDefinitionSet.name + FieldDefinitionFileExtension;
        const QString fullFieldDefinitionSetPath = combinePaths(projectPath, fieldDefinitionSetFileName);

        // Write file.
        QFile fieldDefinitionSetFile(fullFieldDefinitionSetPath);

        if (fieldDefinitionSetFile.open(QIODevice::ReadWrite | QIODevice::Truncate))
        {
            fieldDefinitionSetSerializer.serialize(fieldDefinitionSetFile, fieldDefinitionSet);
        }
        else
        {
            QString errorMessage = QObject::tr("Destination file could not be written:\r\n") + fullFieldDefinitionSetPath;
            throw std::runtime_error(errorMessage.toStdString());
        }
    }

    // Write record sets.
    Tome::RecordSetSerializer recordSetSerializer = RecordSetSerializer();

    for (int i = 0; i < project->recordSets.size(); ++i)
    {
        const RecordSet& recordSet = project->recordSets[i];

        // Build file name.
        const QString recordSetFileName = recordSet.name + RecordFileExtension;
        const QString fullRecordSetPath = Tome::combinePaths(projectPath, recordSetFileName);

        // Write file.
        QFile recordSetFile(fullRecordSetPath);

        if (recordSetFile.open(QIODevice::ReadWrite | QIODevice::Truncate))
        {
            recordSetSerializer.serialize(recordSetFile, recordSet);
        }
        else
        {
            QString errorMessage = QObject::tr("Destination file could not be written:\r\n") + fullRecordSetPath;
            throw std::runtime_error(errorMessage.toStdString());
        }
    }
}

QString Controller::readFile(const QString& path, const QString& fileName)
{
    const QString fullPath = combinePaths(path, fileName);
    QFile file(fullPath);

    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream textStream(&file);
        return textStream.readAll();
    }
    else
    {
        throw std::runtime_error(fullPath.toStdString());
    }
}

void Controller::setProject(QSharedPointer<Project> project)
{
    this->project = project;

    this->componentsController->setComponents(project->components);
    this->exportController->setRecordExportTemplates(project->recordExportTemplates);
    this->fieldDefinitionsController->setFieldDefinitionSets(project->fieldDefinitionSets);
    this->recordsController->setRecordSets(project->recordSets);
    this->typesController->setCustomTypes(project->types);
}

const QString Controller::getFullProjectPath() const
{
    return this->getFullProjectPath(this->project);
}

const QString Controller::getFullProjectPath(QSharedPointer<Project> project) const
{
    if (project == 0)
    {
        return QString();
    }

    return combinePaths(project->path, project->name + ProjectFileExtension);
}
