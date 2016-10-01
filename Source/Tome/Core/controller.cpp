#include "controller.h"

#include <stdexcept>

#include <QFile>
#include <QFileInfo>
#include <QObject>
#include <QTextStream>

#include "commandlineoptions.h"
#include "mainwindow.h"
#include "../Features/Components/Controller/componentscontroller.h"
#include "../Features/Components/Controller/componentsetserializer.h"
#include "../Features/Export/Controller/exportcontroller.h"
#include "../Features/Export/Controller/exporttemplateserializer.h"
#include "../Features/Fields/Controller/fielddefinitionscontroller.h"
#include "../Features/Fields/Controller/fielddefinitionsetserializer.h"
#include "../Features/Integrity/Controller/fieldtypedoesnotexisttask.h"
#include "../Features/Integrity/Controller/listitemtypedoesnotexisttask.h"
#include "../Features/Integrity/Controller/listitemtypenotsupportedtask.h"
#include "../Features/Integrity/Controller/mapkeytypedoesnotexisttask.h"
#include "../Features/Integrity/Controller/mapkeytypenotsupportedtask.h"
#include "../Features/Integrity/Controller/mapvaluetypedoesnotexisttask.h"
#include "../Features/Integrity/Controller/mapvaluetypenotsupportedtask.h"
#include "../Features/Projects/Controller/projectserializer.h"
#include "../Features/Projects/Model/project.h"
#include "../Features/Records/Controller/recordscontroller.h"
#include "../Features/Records/Controller/recordsetserializer.h"
#include "../Features/Search/Controller/findrecordcontroller.h"
#include "../Features/Search/Controller/findusagescontroller.h"
#include "../Features/Settings/Controller/settingscontroller.h"
#include "../Features/Tasks/Controller/taskscontroller.h"
#include "../Features/Types/Controller/typescontroller.h"
#include "../Features/Types/Controller/customtypesetserializer.h"
#include "../Util/pathutils.h"

using namespace Tome;


const QString Controller::ComponentFileExtension = ".tcomp";
const QString Controller::FieldDefinitionFileExtension = ".tfields";
const QString Controller::ProjectFileExtension = ".tproj";
const QString Controller::RecordFileExtension = ".tdata";
const QString Controller::RecordExportComponentTemplateExtension = ".texportc";
const QString Controller::RecordExportComponentDelimiterExtension = ".texportcd";
const QString Controller::RecordExportFieldValueTemplateExtension = ".texportv";
const QString Controller::RecordExportFieldValueDelimiterExtension = ".texportvd";
const QString Controller::RecordExportListTemplateExtension = ".texportl";
const QString Controller::RecordExportListItemTemplateExtension = ".texportli";
const QString Controller::RecordExportListItemDelimiterExtension = ".texportld";
const QString Controller::RecordExportMapTemplateExtension = ".texportm";
const QString Controller::RecordExportMapItemTemplateExtension = ".texportmi";
const QString Controller::RecordExportMapItemDelimiterExtension = ".texportmd";
const QString Controller::RecordExportRecordFileTemplateExtension = ".texportf";
const QString Controller::RecordExportRecordTemplateExtension = ".texportr";
const QString Controller::RecordExportRecordDelimiterExtension = ".texportrd";
const QString Controller::RecordExportTemplateFileExtension = ".texport";
const QString Controller::TypeFileExtension = ".ttypes";

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
    findRecordController(new FindRecordController(*this->recordsController)),
    mainWindow(0)
{
    // Setup tasks.
    this->tasksController->addTask(new FieldTypeDoesNotExistTask());
    this->tasksController->addTask(new ListItemTypeDoesNotExistTask());
    this->tasksController->addTask(new ListItemTypeNotSupportedTask());
    this->tasksController->addTask(new MapKeyTypeDoesNotExistTask());
    this->tasksController->addTask(new MapKeyTypeNotSupportedTask());
    this->tasksController->addTask(new MapValueTypeDoesNotExistTask());
    this->tasksController->addTask(new MapValueTypeNotSupportedTask());
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
    delete this->findRecordController;

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

FindRecordController&Controller::getFindRecordController()
{
    return *this->findRecordController;
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

    // Create component set.
    ComponentSet componentSet = ComponentSet();
    componentSet.name = projectName;
    newProject->componentSets.push_back(componentSet);

    // Create custom type set.
    CustomTypeSet customTypeSet = CustomTypeSet();
    customTypeSet.name = projectName;
    newProject->typeSets.push_back(customTypeSet);

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

bool Controller::getProjectIgnoreReadOnly() const
{
    return this->project->ignoreReadOnly;
}

bool Controller::isProjectLoaded() const
{
    return this->project != 0;
}

void Controller::loadComponentSet(const QString& projectPath, ComponentSet& componentSet)
{
    ComponentSetSerializer componentSerializer = ComponentSetSerializer();

    // TODO(np): Remove as soon as backwards compatibility is removed from ProjectSerializer.
    if (componentSet.components.size() > 0)
    {
        return;
    }

    // Open component file.
    QString fullComponentSetPath =
            buildFullFilePath(componentSet.name, projectPath, ComponentFileExtension);

    QFile componentFile(fullComponentSetPath);
    if (componentFile.open(QIODevice::ReadOnly))
    {
        try
        {
            componentSerializer.deserialize(componentFile, componentSet);
        }
        catch (const std::runtime_error& e)
        {
            QString errorMessage = QObject::tr("File could not be read: ") + fullComponentSetPath + "\r\n" + e.what();
            throw std::runtime_error(errorMessage.toStdString());
        }
    }
    else
    {
        QString errorMessage = QObject::tr("File could not be read:\r\n") + fullComponentSetPath;
        throw std::runtime_error(errorMessage.toStdString());
    }
}

void Controller::loadCustomTypeSet(const QString& projectPath, CustomTypeSet& typeSet)
{
    CustomTypeSetSerializer typesSerializer = CustomTypeSetSerializer();

    // TODO(np): Remove as soon as backwards compatibility is removed from ProjectSerializer.
    if (typeSet.types.size() > 0)
    {
        return;
    }

    // Open types file.
    QString fullTypeSetPath =
            buildFullFilePath(typeSet.name, projectPath, TypeFileExtension);

    QFile typeFile(fullTypeSetPath);
    if (typeFile.open(QIODevice::ReadOnly))
    {
        try
        {
            typesSerializer.deserialize(typeFile, typeSet);
        }
        catch (const std::runtime_error& e)
        {
            QString errorMessage = QObject::tr("File could not be read: ") + fullTypeSetPath + "\r\n" + e.what();
            throw std::runtime_error(errorMessage.toStdString());
        }
    }
    else
    {
        QString errorMessage = QObject::tr("File could not be read:\r\n") + fullTypeSetPath;
        throw std::runtime_error(errorMessage.toStdString());
    }
}

void Controller::loadExportTemplate(const QString& projectPath, RecordExportTemplate& exportTemplate)
{
    ExportTemplateSerializer exportTemplateSerializer = ExportTemplateSerializer();

     // TODO(np): Remove empty check as soon as backwards compatibility is removed from ProjectSerializer.
    if (exportTemplate.fileExtension.isEmpty())
    {
        // Read template file.
        QString fullExportTemplatePath =
                buildFullFilePath(exportTemplate.path, projectPath, RecordExportTemplateFileExtension);

        QFile exportTemplateFile(fullExportTemplatePath);
        if (exportTemplateFile.open(QIODevice::ReadOnly))
        {
            try
            {
                exportTemplateSerializer.deserialize(exportTemplateFile, exportTemplate);
            }
            catch (const std::runtime_error& e)
            {
                QString errorMessage = QObject::tr("File could not be read: ") + fullExportTemplatePath + "\r\n" + e.what();
                throw std::runtime_error(errorMessage.toStdString());
            }
        }
        else
        {
            QString errorMessage = QObject::tr("File could not be read:\r\n") + fullExportTemplatePath;
            throw std::runtime_error(errorMessage.toStdString());
        }
    }

    // Read template contents.
    try
    {
        QString templatePath = exportTemplate.path;

        if (QDir::isRelativePath(templatePath))
        {
            templatePath = combinePaths(projectPath, templatePath);
        }

        if (templatePath.endsWith(RecordExportTemplateFileExtension))
        {
            templatePath = templatePath.remove(RecordExportTemplateFileExtension);
        }

        exportTemplate.fieldValueDelimiter =
                this->readFile(templatePath + RecordExportFieldValueDelimiterExtension);
        exportTemplate.fieldValueTemplate =
                this->readFile(templatePath + RecordExportFieldValueTemplateExtension);
        exportTemplate.recordDelimiter =
                this->readFile(templatePath + RecordExportRecordDelimiterExtension);
        exportTemplate.recordFileTemplate =
                this->readFile(templatePath + RecordExportRecordFileTemplateExtension);
        exportTemplate.recordTemplate =
                this->readFile(templatePath + RecordExportRecordTemplateExtension);
        exportTemplate.componentDelimiter =
                this->readFile(templatePath + RecordExportComponentDelimiterExtension);
        exportTemplate.componentTemplate =
                this->readFile(templatePath + RecordExportComponentTemplateExtension);
        exportTemplate.listTemplate =
                this->readFile(templatePath + RecordExportListTemplateExtension);
        exportTemplate.listItemTemplate =
                this->readFile(templatePath + RecordExportListItemTemplateExtension);
        exportTemplate.listItemDelimiter =
                this->readFile(templatePath + RecordExportListItemDelimiterExtension);
        exportTemplate.mapTemplate =
                this->readFile(templatePath + RecordExportMapTemplateExtension);
        exportTemplate.mapItemTemplate =
                this->readFile(templatePath + RecordExportMapItemTemplateExtension);
        exportTemplate.mapItemDelimiter =
                this->readFile(templatePath + RecordExportMapItemDelimiterExtension);
    }
    catch (const std::runtime_error& e)
    {
        QString errorMessage = QObject::tr("File could not be read:\r\n") + e.what();
        throw std::runtime_error(errorMessage.toStdString());
    }
}

void Controller::loadFieldDefinitionSet(const QString& projectPath, FieldDefinitionSet& fieldDefinitionSet)
{
    FieldDefinitionSetSerializer fieldDefinitionSerializer = FieldDefinitionSetSerializer();

    // Open field definition file.
    QString fullFieldDefinitionSetPath =
            buildFullFilePath(fieldDefinitionSet.name, projectPath, FieldDefinitionFileExtension);

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

void Controller::loadRecordSet(const QString& projectPath, RecordSet& recordSet)
{
    RecordSetSerializer recordSetSerializer = RecordSetSerializer();

    // Open record file.
    QString fullRecordSetPath =
            buildFullFilePath(recordSet.name, projectPath, RecordFileExtension);

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

        // Load component files.
        for (int i = 0; i < project->componentSets.size(); ++i)
        {
            this->loadComponentSet(projectPath, project->componentSets[i]);
        }

        // Load field definition files.
        for (int i = 0; i < project->fieldDefinitionSets.size(); ++i)
        {
            this->loadFieldDefinitionSet(projectPath, project->fieldDefinitionSets[i]);
        }

        // Load record files.
        for (int i = 0; i < project->recordSets.size(); ++i)
        {
            this->loadRecordSet(projectPath, project->recordSets[i]);
        }

        // Load record export template files.
        for (RecordExportTemplateList::iterator it = project->recordExportTemplates.begin();
             it != project->recordExportTemplates.end();
             ++it)
        {
            this->loadExportTemplate(projectPath, *it);
        }

        // Load type files.
        for (int i = 0; i < project->typeSets.size(); ++i)
        {
            this->loadCustomTypeSet(projectPath, project->typeSets[i]);
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

QString Controller::buildFullFilePath(QString filePath, QString projectPath, QString desiredExtension) const
{
    if (!filePath.endsWith(desiredExtension))
    {
        filePath = filePath + desiredExtension;
    }

    if (QDir::isRelativePath(filePath))
    {
        filePath = combinePaths(projectPath, filePath);
    }

    return filePath;
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

    // Write component sets.
    ComponentSetSerializer componentSetSerializer = ComponentSetSerializer();

    for (int i = 0; i < project->componentSets.size(); ++i)
    {
        const ComponentSet& componentSet = project->componentSets[i];

        // Build file name.
        QString fullComponentSetPath =
                buildFullFilePath(componentSet.name, projectPath, ComponentFileExtension);

        // Write file.
        QFile componentSetFile(fullComponentSetPath);

        if (componentSetFile.open(QIODevice::ReadWrite | QIODevice::Truncate))
        {
            componentSetSerializer.serialize(componentSetFile, componentSet);
        }
        else
        {
            QString errorMessage = QObject::tr("Destination file could not be written:\r\n") + fullComponentSetPath;
            throw std::runtime_error(errorMessage.toStdString());
        }
    }

    // Write field definition sets.
    FieldDefinitionSetSerializer fieldDefinitionSetSerializer = FieldDefinitionSetSerializer();

    for (int i = 0; i < project->fieldDefinitionSets.size(); ++i)
    {
        const FieldDefinitionSet& fieldDefinitionSet = project->fieldDefinitionSets[i];

        // Build file name.
        QString fullFieldDefinitionSetPath =
                buildFullFilePath(fieldDefinitionSet.name, projectPath, FieldDefinitionFileExtension);

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
        QString fullRecordSetPath =
                buildFullFilePath(recordSet.name, projectPath, RecordFileExtension);

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

    // Write export templates.
    ExportTemplateSerializer exportTemplateSerializer = ExportTemplateSerializer();

    for (RecordExportTemplateList::const_iterator it = project->recordExportTemplates.begin();
         it != project->recordExportTemplates.end();
         ++it)
    {
        const RecordExportTemplate& exportTemplate = *it;

        // Build file name.
        QString fullExportTemplatePath =
                buildFullFilePath(exportTemplate.path, projectPath, RecordExportTemplateFileExtension);

        // Write file.
        QFile exportTemplateFile(fullExportTemplatePath);

        if (exportTemplateFile.open(QIODevice::ReadWrite | QIODevice::Truncate))
        {
            exportTemplateSerializer.serialize(exportTemplateFile, exportTemplate);
        }
        else
        {
            QString errorMessage = QObject::tr("Destination file could not be written:\r\n") + fullExportTemplatePath;
            throw std::runtime_error(errorMessage.toStdString());
        }
    }

    // Write type sets.
    CustomTypeSetSerializer typeSetSerializer = CustomTypeSetSerializer();

    for (int i = 0; i < project->typeSets.size(); ++i)
    {
        const CustomTypeSet& typeSet = project->typeSets[i];

        // Build file name.
        QString fullTypeSetPath =
                buildFullFilePath(typeSet.name, projectPath, TypeFileExtension);

        // Write file.
        QFile typeSetFile(fullTypeSetPath);

        if (typeSetFile.open(QIODevice::ReadWrite | QIODevice::Truncate))
        {
            typeSetSerializer.serialize(typeSetFile, typeSet);
        }
        else
        {
            QString errorMessage = QObject::tr("Destination file could not be written:\r\n") + fullTypeSetPath;
            throw std::runtime_error(errorMessage.toStdString());
        }
    }
}

QString Controller::readFile(const QString& fullPath)
{
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

    this->componentsController->setComponents(project->componentSets);
    this->exportController->setRecordExportTemplates(project->recordExportTemplates);
    this->fieldDefinitionsController->setFieldDefinitionSets(project->fieldDefinitionSets);
    this->recordsController->setRecordSets(project->recordSets);
    this->typesController->setCustomTypes(project->typeSets);

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
