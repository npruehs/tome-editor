#include "controller.h"

#include <stdexcept>

#include <QApplication>
#include <QFile>
#include <QFileInfo>
#include <QObject>
#include <QSysInfo>
#include <QTextStream>

#include "commandlineoptions.h"
#include "mainwindow.h"
#include "../Features/Components/Controller/componentscontroller.h"
#include "../Features/Components/Controller/componentsetserializer.h"
#include "../Features/Diagnostics/Controller/filemessagehandler.h"
#include "../Features/Diagnostics/Controller/messagehandlers.h"
#include "../Features/Export/Controller/exportcontroller.h"
#include "../Features/Export/Controller/exporttemplateserializer.h"
#include "../Features/Facets/Controller/facetscontroller.h"
#include "../Features/Facets/Controller/maximumintegervaluefacet.h"
#include "../Features/Facets/Controller/maximumrealvaluefacet.h"
#include "../Features/Facets/Controller/maximumstringlengthfacet.h"
#include "../Features/Facets/Controller/minimumintegervaluefacet.h"
#include "../Features/Facets/Controller/minimumrealvaluefacet.h"
#include "../Features/Facets/Controller/requiredreferenceancestorfacet.h"
#include "../Features/Fields/Controller/fielddefinitionscontroller.h"
#include "../Features/Fields/Controller/fielddefinitionsetserializer.h"
#include "../Features/Integrity/Controller/fieldtypedoesnotexisttask.h"
#include "../Features/Integrity/Controller/listitemtypedoesnotexisttask.h"
#include "../Features/Integrity/Controller/listitemtypenotsupportedtask.h"
#include "../Features/Integrity/Controller/mapkeytypedoesnotexisttask.h"
#include "../Features/Integrity/Controller/mapkeytypenotsupportedtask.h"
#include "../Features/Integrity/Controller/mapvaluetypedoesnotexisttask.h"
#include "../Features/Integrity/Controller/mapvaluetypenotsupportedtask.h"
#include "../Features/Integrity/Controller/typefacetviolatedtask.h"
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
#include "../Features/Undo/Controller/undocontroller.h"
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
    undoController(new UndoController()),
    componentsController(new ComponentsController()),
    fieldDefinitionsController(new FieldDefinitionsController()),
    typesController(new TypesController()),
    recordsController(new RecordsController(*this->fieldDefinitionsController, *this->typesController)),
    exportController(new ExportController(*this->fieldDefinitionsController, *this->recordsController, *this->typesController)),
    settingsController(new SettingsController()),
    facetsController(new FacetsController(*this->recordsController, *this->typesController)),
    tasksController(new TasksController(*this->componentsController, *this->facetsController, *this->fieldDefinitionsController, *this->recordsController, *this->typesController)),
    findUsagesController(new FindUsagesController(*this->fieldDefinitionsController, *this->recordsController, *this->typesController)),
    findRecordController(new FindRecordController(*this->recordsController)),
    recordSetSerializer(new RecordSetSerializer()),
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
    this->tasksController->addTask(new TypeFacetViolatedTask());

    // Register facets.
    this->facetsController->registerFacet(new MinimumIntegerValueFacet());
    this->facetsController->registerFacet(new MaximumIntegerValueFacet());
    this->facetsController->registerFacet(new MinimumRealValueFacet());
    this->facetsController->registerFacet(new MaximumRealValueFacet());
    this->facetsController->registerFacet(new MaximumStringLengthFacet());
    this->facetsController->registerFacet(new RequiredReferenceAncestorFacet());

    // Connect signals.
    connect(
                this->recordSetSerializer,
                SIGNAL(progressChanged(QString, QString, int, int)),
                SLOT(onProgressChanged(QString, QString, int, int))
                );
}

Controller::~Controller()
{
    if (this->mainWindow != 0)
    {
        delete this->mainWindow;
    }

    delete this->undoController;
    delete this->componentsController;
    delete this->fieldDefinitionsController;
    delete this->recordsController;
    delete this->exportController;
    delete this->settingsController;
    delete this->typesController;
    delete this->tasksController;
    delete this->findUsagesController;
    delete this->findRecordController;
    delete this->facetsController;
    delete this->recordSetSerializer;

    delete this->options;
}

UndoController&Controller::getUndoController()
{
    return *this->undoController;
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

FacetsController&Controller::getFacetsController()
{
    return *this->facetsController;
}

int Controller::start()
{
    // Install message handlers.
    if (FileMessageHandler::init())
    {
        MessageHandlers::addMessageHandler(FileMessageHandler::handleMessage);
    }

    // Log system information.
    qInfo(QString("Tome Version: %1").arg(QApplication::instance()->applicationVersion()).toUtf8().constData());
    qInfo(QString("Qt Build Architecture: %1").arg(QSysInfo::buildAbi()).toUtf8().constData());
    qInfo(QString("CPU Architecture: %1").arg(QSysInfo::currentCpuArchitecture()).toUtf8().constData());
    qInfo(QString("OS: %1 %2").arg(QSysInfo::prettyProductName(), QSysInfo::kernelVersion()).toUtf8().constData());
    qInfo(QString("Machine Host Name: %1").arg(QSysInfo::machineHostName()).toUtf8().constData());

    if (!this->options->noGui)
    {
        qInfo("Setting up main window.");

        // Setup view.
        this->mainWindow = new MainWindow(this);
        this->mainWindow->show();
    }
    else
    {
        qInfo("Running without main window.");
    }

    if (!this->options->projectPath.isEmpty())
    {
        try
        {
            this->openProject(this->options->projectPath);
        }
        catch (std::runtime_error& e)
        {
            qCritical(e.what());
            return 1;
        }
    }

    if (!this->options->exportTemplateName.isEmpty() &&
            !this->options->exportPath.isEmpty() &&
            this->isProjectLoaded())
    {
        if (!this->exportController->hasRecordExportTemplate(this->options->exportTemplateName))
        {
            qCritical(QString("Export template not found: %1").arg(this->options->exportTemplateName).toUtf8().constData());
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
        catch (std::runtime_error& e)
        {
            qCritical(e.what());
            return 1;
        }
    }

    return 0;
}

void Controller::createProject(const QString& projectName, const QString& projectPath)
{
    qInfo(QString("Creating new project %1 at %2.").arg(projectName, projectPath).toUtf8().constData());

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

    qInfo(QString("Opening components file %1.").arg(fullComponentSetPath).toUtf8().constData());

    if (componentFile.open(QIODevice::ReadOnly))
    {
        try
        {
            componentSerializer.deserialize(componentFile, componentSet);
            qInfo(QString("Opened components file %1 with %2 components.")
                  .arg(fullComponentSetPath, QString::number(componentSet.components.count())).toUtf8().constData());
        }
        catch (const std::runtime_error& e)
        {
            QString errorMessage = QObject::tr("File could not be read: ") + fullComponentSetPath + "\r\n" + e.what();
            qCritical(errorMessage.toUtf8().constData());
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

    qInfo(QString("Opening types file %1.").arg(fullTypeSetPath).toUtf8().constData());

    if (typeFile.open(QIODevice::ReadOnly))
    {
        try
        {
            typesSerializer.deserialize(typeFile, typeSet);
            qInfo(QString("Opened types file %1 with %2 custom types.")
                  .arg(fullTypeSetPath, QString::number(typeSet.types.count())).toUtf8().constData());
        }
        catch (const std::runtime_error& e)
        {
            QString errorMessage = QObject::tr("File could not be read: ") + fullTypeSetPath + "\r\n" + e.what();
            qCritical(errorMessage.toUtf8().constData());
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

        qInfo(QString("Opening export template file %1.").arg(fullExportTemplatePath).toUtf8().constData());

        if (exportTemplateFile.open(QIODevice::ReadOnly))
        {
            try
            {
                exportTemplateSerializer.deserialize(exportTemplateFile, exportTemplate);
            }
            catch (const std::runtime_error& e)
            {
                QString errorMessage = QObject::tr("File could not be read: ") + fullExportTemplatePath + "\r\n" + e.what();
                qCritical(errorMessage.toUtf8().constData());
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
        qCritical(errorMessage.toUtf8().constData());
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

    qInfo(QString("Opening field definitions file %1.").arg(fullFieldDefinitionSetPath).toUtf8().constData());

    if (fieldDefinitionFile.open(QIODevice::ReadOnly))
    {
        try
        {
            fieldDefinitionSerializer.deserialize(fieldDefinitionFile, fieldDefinitionSet);
            qInfo(QString("Opened field definitions file %1 with %2 fields.")
                  .arg(fullFieldDefinitionSetPath, QString::number(fieldDefinitionSet.fieldDefinitions.count())).toUtf8().constData());
        }
        catch (const std::runtime_error& e)
        {
            QString errorMessage = QObject::tr("File could not be read: ") + fullFieldDefinitionSetPath + "\r\n" + e.what();
            qCritical(errorMessage.toUtf8().constData());
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
    // Open record file.
    QString fullRecordSetPath =
            buildFullFilePath(recordSet.name, projectPath, RecordFileExtension);

    QFile recordFile(fullRecordSetPath);

    qInfo(QString("Opening records file %1.").arg(fullRecordSetPath).toUtf8().constData());

    if (recordFile.open(QIODevice::ReadOnly))
    {
        try
        {
            this->recordSetSerializer->deserialize(recordFile, recordSet);
            qInfo(QString("Opened records file %1 with %2 records.")
                  .arg(fullRecordSetPath, QString::number(recordSet.records.count())).toUtf8().constData());
        }
        catch (const std::runtime_error& e)
        {
            QString errorMessage = QObject::tr("File could not be read: ") + fullRecordSetPath + "\r\n" + e.what();
            qCritical(errorMessage.toUtf8().constData());
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

    qInfo(QString("Opening project %1.").arg(projectFileName).toUtf8().constData());

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
            qCritical(errorMessage.toUtf8().constData());
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

void Controller::onProgressChanged(const QString title, const QString text, const int currentValue, const int maximumValue)
{
    emit this->progressChanged(title, text, currentValue, maximumValue);
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

    qInfo(QString("Saving project %1.").arg(fullProjectPath).toUtf8().constData());

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

        qInfo(QString("Saving components file %1.").arg(fullComponentSetPath).toUtf8().constData());

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

        qInfo(QString("Saving field definitions file %1.").arg(fullFieldDefinitionSetPath).toUtf8().constData());

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
    for (int i = 0; i < project->recordSets.size(); ++i)
    {
        const RecordSet& recordSet = project->recordSets[i];

        // Build file name.
        QString fullRecordSetPath =
                buildFullFilePath(recordSet.name, projectPath, RecordFileExtension);

        // Write file.
        QFile recordSetFile(fullRecordSetPath);

        qInfo(QString("Saving records file %1.").arg(fullRecordSetPath).toUtf8().constData());

        if (recordSetFile.open(QIODevice::ReadWrite | QIODevice::Truncate))
        {
            this->recordSetSerializer->serialize(recordSetFile, recordSet);
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

        qInfo(QString("Saving export template file %1.").arg(fullExportTemplatePath).toUtf8().constData());

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

        qInfo(QString("Saving types file %1.").arg(fullTypeSetPath).toUtf8().constData());

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
