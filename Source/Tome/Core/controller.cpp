#include "controller.h"

#include <stdexcept>

#include <QFile>
#include <QFileInfo>
#include <QObject>
#include <QTextStream>

#include "commandlineoptions.h"
#include "mainwindow.h"
#include "../Features/Components/Controller/componentscontroller.h"
#include "../Features/Export/Controller/exportcontroller.h"
#include "../Features/Fields/Controller/fielddefinitionscontroller.h"
#include "../Features/Fields/Controller/fielddefinitionsetserializer.h"
#include "../Features/Integrity/Controller/fieldtypedoesnotexisttask.h"
#include "../Features/Integrity/Controller/listitemtypedoesnotexisttask.h"
#include "../Features/Integrity/Controller/listitemtypenotsupportedtask.h"
#include "../Features/Projects/Controller/projectserializer.h"
#include "../Features/Projects/Model/project.h"
#include "../Features/Records/Controller/recordscontroller.h"
#include "../Features/Records/Controller/recordsetserializer.h"
#include "../Features/Search/Controller/findusagescontroller.h"
#include "../Features/Settings/Controller/settingscontroller.h"
#include "../Features/Tasks/Controller/taskscontroller.h"
#include "../Features/Types/Controller/typescontroller.h"
#include "../Util/pathutils.h"

using namespace Tome;


const QString Controller::FieldDefinitionFileExtension = ".tfields";
const QString Controller::ProjectFileExtension = ".tproj";
const QString Controller::RecordFileExtension = ".tdata";
const QString Controller::RecordExportComponentTemplateExtension = ".texportc";
const QString Controller::RecordExportComponentDelimiterExtension = ".texportcd";
const QString Controller::RecordExportListTemplateExtension = ".texportl";
const QString Controller::RecordExportListItemTemplateExtension = ".texportli";
const QString Controller::RecordExportListItemDelimiterExtension = ".texportld";
const QString Controller::RecordExportMapTemplateExtension = ".texportm";
const QString Controller::RecordExportMapItemTemplateExtension = ".texportmi";
const QString Controller::RecordExportMapItemDelimiterExtension = ".texportmd";
const QString Controller::RecordExportRecordFileTemplateExtension = ".texportf";
const QString Controller::RecordExportRecordTemplateExtension = ".texportr";
const QString Controller::RecordExportRecordDelimiterExtension = ".texportrd";
const QString Controller::RecordExportFieldValueTemplateExtension = ".texportv";
const QString Controller::RecordExportFieldValueDelimiterExtension = ".texportvd";


Controller::Controller(CommandLineOptions* options) :
    options(options),
    componentsController(new ComponentsController()),
    fieldDefinitionsController(new FieldDefinitionsController()),
    typesController(new TypesController()),
    recordsController(new RecordsController(*this->fieldDefinitionsController)),
    exportController(new ExportController(*this->fieldDefinitionsController, *this->recordsController, *this->typesController)),
    settingsController(new SettingsController()),
    tasksController(new TasksController(*this->componentsController, *this->fieldDefinitionsController, *this->recordsController, *this->typesController)),
    findUsagesController(new FindUsagesController(*this->fieldDefinitionsController, *this->recordsController, *this->typesController)),
    mainWindow(0)
{
    // Setup tasks.
    this->tasksController->addTask(new FieldTypeDoesNotExistTask());
    this->tasksController->addTask(new ListItemTypeDoesNotExistTask());
    this->tasksController->addTask(new ListItemTypeNotSupportedTask());
}

Controller::~Controller()
{
    if (this->mainWindow != 0)
    {
        delete this->mainWindow;
    }

    delete this->componentsController;
    delete this->fieldDefinitionsController;
    delete this->recordsController;
    delete this->exportController;
    delete this->settingsController;
    delete this->typesController;
    delete this->tasksController;
    delete this->findUsagesController;

    delete this->options;
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

TasksController& Controller::getTasksController()
{
    return *this->tasksController;
}

TypesController& Controller::getTypesController()
{
    return *this->typesController;
}

FindUsagesController& Controller::getFindUsagesController()
{
    return *this->findUsagesController;
}

int Controller::start()
{
    if (!this->options->noGui)
    {
        // Setup view.
        this->mainWindow = new MainWindow(this);
        this->mainWindow->show();
    }

    if (!this->options->projectPath.isEmpty())
    {
        try
        {
            this->openProject(this->options->projectPath);
        }
        catch (std::runtime_error&)
        {
            // TODO(np): Write error log.
            return 1;
        }
    }

    if (!this->options->exportTemplateName.isEmpty() &&
            !this->options->exportPath.isEmpty() &&
            this->isProjectLoaded())
    {
        if (!this->exportController->hasRecordExportTemplate(this->options->exportTemplateName))
        {
            // TODO(np): Write error log.
            return 1;
        }

        // Get export template.
        const Tome::RecordExportTemplate& exportTemplate =
                this->exportController->getRecordExportTemplate(this->options->exportTemplateName);

        // Build export file path.
        const QString filePath = this->options->exportPath + exportTemplate.fileExtension;

        // Export records.
        try
        {
            this->exportController->exportRecords(exportTemplate, filePath);
        }
        catch (std::runtime_error&)
        {
            // TODO(np): Write error log.
            return 1;
        }
    }

    return 0;
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
            QString fullFieldDefinitionSetPath = fieldDefinitionSet.name;

            if (!fullFieldDefinitionSetPath.endsWith(FieldDefinitionFileExtension))
            {
                fullFieldDefinitionSetPath = fullFieldDefinitionSetPath + FieldDefinitionFileExtension;
            }

            if (QDir::isRelativePath(fullFieldDefinitionSetPath))
            {
                fullFieldDefinitionSetPath = combinePaths(projectPath, fullFieldDefinitionSetPath);
            }

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
            QString fullRecordSetPath = recordSet.name;

            if (!fullRecordSetPath.endsWith(RecordFileExtension))
            {
                fullRecordSetPath = fullRecordSetPath + RecordFileExtension;
            }

            if (QDir::isRelativePath(fullRecordSetPath))
            {
                fullRecordSetPath = combinePaths(projectPath, fullRecordSetPath);
            }

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
                QString templatePath;
                if ( !exportTemplate.path.isEmpty() )
                {
                    if (QDir::isRelativePath(exportTemplate.path))
                    {
                        templatePath = combinePaths( projectPath, exportTemplate.path );
                    }
                    else
                    {
                        templatePath = exportTemplate.path;
                    }
                }
                else
                {
                    templatePath = projectPath;
                }
                exportTemplate.fieldValueDelimiter =
                        this->readFile(templatePath, exportTemplate.name + RecordExportFieldValueDelimiterExtension);
                exportTemplate.fieldValueTemplate =
                        this->readFile(templatePath, exportTemplate.name + RecordExportFieldValueTemplateExtension);
                exportTemplate.recordDelimiter =
                        this->readFile(templatePath, exportTemplate.name + RecordExportRecordDelimiterExtension);
                exportTemplate.recordFileTemplate =
                        this->readFile(templatePath, exportTemplate.name + RecordExportRecordFileTemplateExtension);
                exportTemplate.recordTemplate =
                        this->readFile(templatePath, exportTemplate.name + RecordExportRecordTemplateExtension);
                exportTemplate.componentDelimiter =
                        this->readFile(templatePath, exportTemplate.name + RecordExportComponentDelimiterExtension);
                exportTemplate.componentTemplate =
                        this->readFile(templatePath, exportTemplate.name + RecordExportComponentTemplateExtension);
                exportTemplate.listTemplate =
                        this->readFile(templatePath, exportTemplate.name + RecordExportListTemplateExtension);
                exportTemplate.listItemTemplate =
                        this->readFile(templatePath, exportTemplate.name + RecordExportListItemTemplateExtension);
                exportTemplate.listItemDelimiter =
                        this->readFile(templatePath, exportTemplate.name + RecordExportListItemDelimiterExtension);
                exportTemplate.mapTemplate =
                        this->readFile(templatePath, exportTemplate.name + RecordExportMapTemplateExtension);
                exportTemplate.mapItemTemplate =
                        this->readFile(templatePath, exportTemplate.name + RecordExportMapItemTemplateExtension);
                exportTemplate.mapItemDelimiter =
                        this->readFile(templatePath, exportTemplate.name + RecordExportMapItemDelimiterExtension);
            }
            catch (const std::runtime_error& e)
            {
                QString errorMessage = QObject::tr("File could not be read:\r\n") + e.what();
                throw std::runtime_error(errorMessage.toStdString());
            }
        }

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
        QString fullFieldDefinitionSetPath = fieldDefinitionSet.name;

        if (!fullFieldDefinitionSetPath.endsWith(FieldDefinitionFileExtension))
        {
            fullFieldDefinitionSetPath = fullFieldDefinitionSetPath + FieldDefinitionFileExtension;
        }

        if (QDir::isRelativePath(fullFieldDefinitionSetPath))
        {
            fullFieldDefinitionSetPath = combinePaths(projectPath, fullFieldDefinitionSetPath);
        }

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
        QString fullRecordSetPath = recordSet.name;

        if (!fullRecordSetPath.endsWith(RecordFileExtension))
        {
            fullRecordSetPath = fullRecordSetPath + RecordFileExtension;
        }

        if (QDir::isRelativePath(fullRecordSetPath))
        {
            fullRecordSetPath = combinePaths(projectPath, fullRecordSetPath);
        }

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

    // Add to recent projects.
    const QString& fullPath = this->getFullProjectPath();
    this->settingsController->addRecentProject(fullPath);

    // Set the default locale.
    QLocale::setDefault(project->locale);

    // Notify listeners.
    emit projectChanged(this->project);
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
