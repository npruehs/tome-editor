#include "controller.h"

#include <stdexcept>

#include <QApplication>
#include <QFileInfo>
#include <QSysInfo>

#include "commandlineoptions.h"
#include "mainwindow.h"
#include "../Features/Components/Controller/componentscontroller.h"
#include "../Features/Diagnostics/Controller/filemessagehandler.h"
#include "../Features/Diagnostics/Controller/messagehandlers.h"
#include "../Features/Export/Controller/exportcontroller.h"
#include "../Features/Facets/Controller/facetscontroller.h"
#include "../Features/Facets/Controller/localizedstringfacet.h"
#include "../Features/Facets/Controller/maximumintegervaluefacet.h"
#include "../Features/Facets/Controller/maximumrealvaluefacet.h"
#include "../Features/Facets/Controller/maximumstringlengthfacet.h"
#include "../Features/Facets/Controller/minimumintegervaluefacet.h"
#include "../Features/Facets/Controller/minimumrealvaluefacet.h"
#include "../Features/Facets/Controller/removedfileprefixfacet.h"
#include "../Features/Facets/Controller/removedfilesuffixfacet.h"
#include "../Features/Facets/Controller/requiredreferenceancestorfacet.h"
#include "../Features/Fields/Controller/fielddefinitionscontroller.h"
#include "../Features/Import/Controller/importcontroller.h"
#include "../Features/Integrity/Controller/componenthasnofieldstask.h"
#include "../Features/Integrity/Controller/fieldalwayshasitsdefaultvaluetask.h"
#include "../Features/Integrity/Controller/fieldisneverusedtask.h"
#include "../Features/Integrity/Controller/fieldtypedoesnotexisttask.h"
#include "../Features/Integrity/Controller/listitemtypedoesnotexisttask.h"
#include "../Features/Integrity/Controller/listitemtypenotsupportedtask.h"
#include "../Features/Integrity/Controller/mapkeytypedoesnotexisttask.h"
#include "../Features/Integrity/Controller/mapkeytypenotsupportedtask.h"
#include "../Features/Integrity/Controller/mapvaluetypedoesnotexisttask.h"
#include "../Features/Integrity/Controller/mapvaluetypenotsupportedtask.h"
#include "../Features/Integrity/Controller/referencedfiledoesnotexisttask.h"
#include "../Features/Integrity/Controller/referencedrecorddoesnotexisttask.h"
#include "../Features/Integrity/Controller/typefacetviolatedtask.h"
#include "../Features/Integrity/Controller/typeisneverusedtask.h"
#include "../Features/Projects/Controller/projectcontroller.h"
#include "../Features/Projects/Model/project.h"
#include "../Features/Records/Controller/recordscontroller.h"
#include "../Features/Search/Controller/findrecordcontroller.h"
#include "../Features/Search/Controller/findusagescontroller.h"
#include "../Features/Settings/Controller/settingscontroller.h"
#include "../Features/Tasks/Controller/taskscontroller.h"
#include "../Features/Types/Controller/typescontroller.h"
#include "../Features/Undo/Controller/undocontroller.h"


using namespace Tome;


Controller::Controller(CommandLineOptions* options) :
    options(options),
    projectController(new ProjectController()),
    undoController(new UndoController()),
    componentsController(new ComponentsController()),
    typesController(new TypesController()),
    fieldDefinitionsController(new FieldDefinitionsController(*this->componentsController, *this->typesController)),
    recordsController(new RecordsController(*this->fieldDefinitionsController, *this->projectController, *this->typesController)),
    facetsController(new FacetsController(*this->recordsController, *this->typesController)),
    exportController(new ExportController(*this->facetsController, *this->fieldDefinitionsController, *this->recordsController, *this->typesController)),
    settingsController(new SettingsController()),
    tasksController(new TasksController(*this->componentsController, *this->facetsController, *this->fieldDefinitionsController, *this->projectController, *this->recordsController, *this->typesController)),
    findUsagesController(new FindUsagesController(*this->fieldDefinitionsController, *this->recordsController, *this->typesController)),
    findRecordController(new FindRecordController(*this->recordsController)),
    importController(new ImportController(*this->fieldDefinitionsController, *this->recordsController, *this->typesController)),
    mainWindow(0)
{
    // Setup tasks.
    this->tasksController->addTask(new ComponentHasNoFieldsTask());
    this->tasksController->addTask(new FieldAlwaysHasItsDefaultValueTask());
    this->tasksController->addTask(new FieldIsNeverUsedTask());
    this->tasksController->addTask(new FieldTypeDoesNotExistTask());
    this->tasksController->addTask(new ListItemTypeDoesNotExistTask());
    this->tasksController->addTask(new ListItemTypeNotSupportedTask());
    this->tasksController->addTask(new MapKeyTypeDoesNotExistTask());
    this->tasksController->addTask(new MapKeyTypeNotSupportedTask());
    this->tasksController->addTask(new MapValueTypeDoesNotExistTask());
    this->tasksController->addTask(new MapValueTypeNotSupportedTask());
    this->tasksController->addTask(new ReferencedFileDoesNotExistTask());
    this->tasksController->addTask(new ReferencedRecordDoesNotExistTask());
    this->tasksController->addTask(new TypeFacetViolatedTask());
    this->tasksController->addTask(new TypeIsNeverUsedTask());

    // Register facets.
    this->facetsController->registerFacet(new LocalizedStringFacet());
    this->facetsController->registerFacet(new MinimumIntegerValueFacet());
    this->facetsController->registerFacet(new MaximumIntegerValueFacet());
    this->facetsController->registerFacet(new MinimumRealValueFacet());
    this->facetsController->registerFacet(new MaximumRealValueFacet());
    this->facetsController->registerFacet(new MaximumStringLengthFacet());
    this->facetsController->registerFacet(new RemovedFilePrefixFacet());
    this->facetsController->registerFacet(new RemovedFileSuffixFacet());
    this->facetsController->registerFacet(new RequiredReferenceAncestorFacet());

    // Connect signals.
    connect(
                this->projectController,
                SIGNAL(projectChanged(QSharedPointer<Tome::Project>)),
                SLOT(onProjectChanged(QSharedPointer<Tome::Project>))
                );
}

Controller::~Controller()
{
    if (this->mainWindow != 0)
    {
        delete this->mainWindow;
    }

    delete this->projectController;
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
    delete this->importController;

    delete this->options;
}

ProjectController&Controller::getProjectController() const
{
    return *this->projectController;
}

UndoController&Controller::getUndoController() const
{
    return *this->undoController;
}

ComponentsController& Controller::getComponentsController() const
{
    return *this->componentsController;
}

FieldDefinitionsController& Controller::getFieldDefinitionsController() const
{
    return *this->fieldDefinitionsController;
}

RecordsController& Controller::getRecordsController() const
{
    return *this->recordsController;
}

ExportController& Controller::getExportController() const
{
    return *this->exportController;
}

SettingsController& Controller::getSettingsController() const
{
    return *this->settingsController;
}

TasksController& Controller::getTasksController() const
{
    return *this->tasksController;
}

TypesController& Controller::getTypesController() const
{
    return *this->typesController;
}

FindUsagesController& Controller::getFindUsagesController() const
{
    return *this->findUsagesController;
}

FindRecordController&Controller::getFindRecordController() const
{
    return *this->findRecordController;
}

FacetsController&Controller::getFacetsController() const
{
    return *this->facetsController;
}

ImportController&Controller::getImportController() const
{
    return *this->importController;
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
    qInfo(QString("Log File: %1").arg(QFileInfo(FileMessageHandler::logfileName).absoluteFilePath()).toUtf8().constData());
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
            this->projectController->openProject(this->options->projectPath);
        }
        catch (std::runtime_error& e)
        {
            qCritical(e.what());
            return 1;
        }
    }

    if (!this->options->exportTemplateName.isEmpty() &&
            !this->options->exportPath.isEmpty() &&
            this->projectController->isProjectLoaded())
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

void Controller::onProjectChanged(QSharedPointer<Project> project)
{
    this->componentsController->setComponents(project->componentSets);
    this->exportController->setRecordExportTemplates(project->recordExportTemplates);
    this->fieldDefinitionsController->setFieldDefinitionSets(project->fieldDefinitionSets);
    this->recordsController->setRecordSets(project->recordSets);
    this->typesController->setCustomTypes(project->typeSets);
    this->importController->setRecordTableImportTemplates(project->recordTableImportTemplates);

    // Add to recent projects.
    const QString& fullPath = this->projectController->getFullProjectPath();
    this->settingsController->addRecentProject(fullPath);

    // Set the default locale.
    QLocale::setDefault(project->locale);

    // Reset undo stack.
    this->undoController->clear();
}
