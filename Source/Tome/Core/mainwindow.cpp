#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <stdexcept>

#include <QDesktopServices>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QInputDialog>
#include <QLabel>
#include <QMessageBox>
#include <QProcess>
#include <QProgressDialog>
#include <QStandardPaths>
#include <QTextStream>
#include <QXmlStreamWriter>

#include "controller.h"
#include "../Features/Components/View/componentswindow.h"
#include "../Features/Components/Controller/componentscontroller.h"
#include "../Features/Diagnostics/View/outputdockwidget.h"
#include "../Features/Export/Controller/exportcontroller.h"
#include "../Features/Facets/Controller/facet.h"
#include "../Features/Facets/Controller/facetscontroller.h"
#include "../Features/Fields/Controller/fielddefinitionsetserializer.h"
#include "../Features/Fields/Controller/fielddefinitionscontroller.h"
#include "../Features/Fields/View/fielddefinitionswindow.h"
#include "../Features/Fields/View/fieldvaluewindow.h"
#include "../Features/Help/View/aboutwindow.h"
#include "../Features/Import/Controller/importcontroller.h"
#include "../Features/Import/Model/recordtableimporttemplatelist.h"
#include "../Features/Projects/Model/project.h"
#include "../Features/Projects/Controller/projectcontroller.h"
#include "../Features/Projects/Controller/projectserializer.h"
#include "../Features/Projects/View/newprojectwindow.h"
#include "../Features/Projects/View/projectoverviewwindow.h"
#include "../Features/Records/Controller/recordscontroller.h"
#include "../Features/Records/Controller/recordsetserializer.h"
#include "../Features/Records/Controller/Commands/addrecordcommand.h"
#include "../Features/Records/Controller/Commands/duplicaterecordcommand.h"
#include "../Features/Records/Controller/Commands/reparentrecordcommand.h"
#include "../Features/Records/Controller/Commands/removerecordcommand.h"
#include "../Features/Records/Controller/Commands/revertrecordcommand.h"
#include "../Features/Records/Controller/Commands/updaterecordcommand.h"
#include "../Features/Records/Controller/Commands/updaterecordfieldvaluecommand.h"
#include "../Features/Records/Model/recordfieldstate.h"
#include "../Features/Records/View/recordfieldstablewidget.h"
#include "../Features/Records/View/recordtreewidget.h"
#include "../Features/Records/View/recordtreewidgetitem.h"
#include "../Features/Records/View/recordwindow.h"
#include "../Features/Records/View/duplicaterecordwindow.h"
#include "../Features/Search/Controller/findrecordcontroller.h"
#include "../Features/Search/Controller/findusagescontroller.h"
#include "../Features/Search/View/findrecordwindow.h"
#include "../Features/Search/View/searchresultsdockwidget.h"
#include "../Features/Settings/Controller/settingscontroller.h"
#include "../features/Settings/View/usersettingswindow.h"
#include "../Features/Tasks/Controller/taskscontroller.h"
#include "../Features/Tasks/Model/severity.h"
#include "../Features/Tasks/Model/targetsitetype.h"
#include "../Features/Tasks/View/errorlistdockwidget.h"
#include "../Features/Types/Controller/typescontroller.h"
#include "../Features/Types/Model/builtintype.h"
#include "../Features/Types/View/customtypeswindow.h"
#include "../Features/Undo/Controller/undocontroller.h"
#include "../Util/listutils.h"
#include "../Util/pathutils.h"
#include "../Util/stringutils.h"


using namespace Tome;


MainWindow::MainWindow(Controller* controller, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    controller(controller),
    aboutWindow(0),
    componentsWindow(0),
    customTypesWindow(0),
    fieldDefinitionsWindow(0),
    fieldValueWindow(0),
    newProjectWindow(0),
    recordWindow(0),
    duplicateRecordWindow(0),
    findRecordWindow(0),
    projectOverviewWindow(0),
    userSettingsWindow(0),
    progressDialog(0),
    refreshRecordTreeAfterReparent(true)
{
    ui->setupUi(this);

    // Add record tree.
    this->recordTreeWidget = new RecordTreeWidget(this->controller->getRecordsController(),
                                                  this->controller->getFacetsController(),
                                                  this->controller->getFieldDefinitionsController(),
                                                  this->controller->getProjectController(),
                                                  this->controller->getSettingsController());
    this->ui->splitter->addWidget(this->recordTreeWidget);

    // Setup record tree context menu.
    QList<QAction*> recordTreeContextMenuActions;
    recordTreeContextMenuActions << this->ui->actionNew_Record;
    recordTreeContextMenuActions << this->ui->actionAdd_Child;
    recordTreeContextMenuActions << this->ui->actionFind_Usages;
    recordTreeContextMenuActions << this->ui->actionEdit_Record;
    recordTreeContextMenuActions << this->ui->actionDuplicate_Record;
    recordTreeContextMenuActions << this->ui->actionRevert_Record;
    recordTreeContextMenuActions << this->ui->actionRemove_Record;

    this->recordTreeWidget->setContextMenuActions(recordTreeContextMenuActions);

    // Add record table.
    this->recordFieldTableWidget = new RecordFieldsTableWidget(
                this->controller->getFieldDefinitionsController(),
                this->controller->getFacetsController(),
                this->controller->getProjectController(),
                this->controller->getRecordsController(),
                this->controller->getTypesController());

    this->ui->splitter->addWidget(this->recordFieldTableWidget);

    // Add search results.
    this->searchResultsDockWidget = new SearchResultsDockWidget(this);
    this->addDockWidget(Qt::BottomDockWidgetArea, this->searchResultsDockWidget, Qt::Vertical);
    this->dockWidgets.push_back(this->searchResultsDockWidget);

    // Add error list.
    this->errorListDockWidget = new ErrorListDockWidget(this);
    this->addDockWidget(Qt::BottomDockWidgetArea, this->errorListDockWidget, Qt::Vertical);
    this->dockWidgets.push_back(this->errorListDockWidget);

    // Add log window.
    this->outputDockWidget = new OutputDockWidget(this);
    this->outputDockWidget->init();
    this->addDockWidget(Qt::BottomDockWidgetArea, this->outputDockWidget, Qt::Vertical);
    this->dockWidgets.push_back(this->outputDockWidget);

    // Hide most dock widgets until required.
    this->searchResultsDockWidget->close();
    this->errorListDockWidget->close();

    // Connect signals.
    connect(
                &this->controller->getProjectController(),
                SIGNAL(projectChanged(QSharedPointer<Tome::Project>)),
                SLOT(onProjectChanged(QSharedPointer<Tome::Project>))
                );

    connect(
                &this->controller->getRecordsController(),
                SIGNAL(recordAdded(const QVariant&, const QString&, const QVariant&)),
                SLOT(onRecordAdded(const QVariant&, const QString&, const QVariant&))
                );

    connect(
                &this->controller->getRecordsController(),
                SIGNAL(recordFieldsChanged(const QVariant&)),
                SLOT(onRecordFieldsChanged(const QVariant&))
                );

    connect(
                &this->controller->getRecordsController(),
                SIGNAL(recordRemoved(const QVariant&)),
                SLOT(onRecordRemoved(const QVariant&))
                );

    connect(
                &this->controller->getRecordsController(),
                SIGNAL(recordReparented(const QVariant&, const QVariant&, const QVariant&)),
                SLOT(onRecordReparented(const QVariant&, const QVariant&, const QVariant&))
                );

    connect(
                &this->controller->getRecordsController(),
                SIGNAL(recordSetsChanged()),
                SLOT(onRecordSetsChanged())
                );

    connect(
                &this->controller->getRecordsController(),
                SIGNAL(recordUpdated(const QVariant&, const QString&, const QString&, const QVariant&, const QString&, const QString&)),
                SLOT(onRecordUpdated(const QVariant&, const QString&, const QString&, const QVariant&, const QString&, const QString&))
                );

    connect(
                &this->controller->getRecordsController(),
                SIGNAL(progressChanged(QString,QString,int,int)),
                SLOT(onProgressChanged(QString,QString,int,int))
                );

    connect(
                &this->controller->getExportController(),
                SIGNAL(exportTemplatesChanged()),
                SLOT(onExportTemplatesChanged())
                );

    connect(
                &this->controller->getExportController(),
                SIGNAL(progressChanged(QString,QString,int,int)),
                SLOT(onProgressChanged(QString,QString,int,int))
                );

    connect(
                this->ui->menuExport,
                SIGNAL(triggered(QAction*)),
                SLOT(exportRecords(QAction*))
                );

    connect(
                this->ui->menuImport,
                SIGNAL(triggered(QAction*)),
                SLOT(importRecords(QAction*))
                );

    connect(
                this->ui->menuRecent_Projects,
                SIGNAL(triggered(QAction*)),
                SLOT(openRecentProject(QAction*))
                );

    connect(
                this->recordFieldTableWidget,
                SIGNAL(doubleClicked(const QModelIndex &)),
                SLOT(tableWidgetDoubleClicked(const QModelIndex &))
                );

    connect(
                this->recordTreeWidget,
                SIGNAL(doubleClicked(const QModelIndex &)),
                SLOT(treeWidgetDoubleClicked(const QModelIndex &))
                );

    connect(
                this->recordTreeWidget->selectionModel(),
                SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
                SLOT(treeWidgetSelectionChanged(const QItemSelection &, const QItemSelection &))
                );

    connect(
                this->recordTreeWidget,
                SIGNAL(recordReparented(const QVariant&, const QVariant&)),
                SLOT(treeWidgetRecordReparented(const QVariant&, const QVariant&))
                );

    connect(
                this->recordFieldTableWidget,
                SIGNAL(fileLinkActivated(const QString&)),
                SLOT(onFileLinkActivated(const QString&))
                );

    connect(
                this->recordFieldTableWidget,
                SIGNAL(recordLinkActivated(const QString&)),
                SLOT(onRecordLinkActivated(const QString&))
                );

    connect(
                &this->controller->getFindUsagesController(),
                SIGNAL(searchResultChanged(const QString&, const Tome::SearchResultList)),
                SLOT(searchResultChanged(const QString&, const Tome::SearchResultList))
                );

    connect(
                &this->controller->getFindUsagesController(),
                SIGNAL(progressChanged(QString,QString,int,int)),
                SLOT(onProgressChanged(QString,QString,int,int))
                );

    connect(
                &this->controller->getFindRecordController(),
                SIGNAL(searchResultChanged(const QString&, const Tome::SearchResultList)),
                SLOT(searchResultChanged(const QString&, const Tome::SearchResultList))
                );

    connect(
                &this->controller->getFindRecordController(),
                SIGNAL(progressChanged(QString,QString,int,int)),
                SLOT(onProgressChanged(QString,QString,int,int))
                );

    connect(
                this->searchResultsDockWidget,
                SIGNAL(recordLinkActivated(const QString&)),
                SLOT(onRecordLinkActivated(const QString&))
                );

    connect(
                this->searchResultsDockWidget,
                SIGNAL(progressChanged(QString,QString,int,int)),
                SLOT(onProgressChanged(QString,QString,int,int))
                );

    connect(
                &this->controller->getProjectController(),
                SIGNAL(progressChanged(QString, QString, int, int)),
                SLOT(onProgressChanged(QString, QString, int, int))
                );

    connect(
                this->recordTreeWidget,
                SIGNAL(progressChanged(QString,QString,int,int)),
                SLOT(onProgressChanged(QString,QString,int,int))
                );

    connect(
                this->errorListDockWidget,
                SIGNAL(progressChanged(QString,QString,int,int)),
                SLOT(onProgressChanged(QString,QString,int,int))
                );

    connect(
                this->errorListDockWidget,
                SIGNAL(recordLinkActivated(const QString&)),
                SLOT(onRecordLinkActivated(const QString&))
                );

    connect(
                &this->controller->getUndoController(),
                SIGNAL(undoStackChanged(bool)),
                SLOT(onUndoStackChanged(bool))
                );

    connect(
                &this->controller->getImportController(),
                SIGNAL(importTemplatesChanged()),
                SLOT(onImportTemplatesChanged())
                );

    connect(
                &this->controller->getImportController(),
                SIGNAL(importError(const QString&)),
                SLOT(onImportError(const QString&))
                );

    connect(
                &this->controller->getImportController(),
                SIGNAL(importFinished()),
                SLOT(onImportFinished())
                );

    connect(
                &this->controller->getImportController(),
                SIGNAL(importStarted()),
                SLOT(onImportStarted())
                );

    connect(
                &this->controller->getImportController(),
                SIGNAL(progressChanged(QString,QString,int,int)),
                SLOT(onProgressChanged(QString,QString,int,int))
                );

    connect(
                &this->controller->getTasksController(),
                SIGNAL(progressChanged(QString,QString,int,int)),
                SLOT(onProgressChanged(QString,QString,int,int))
                );

    // Maximize window.
    this->showMaximized();

    // Set window title.
    this->updateWindowTitle();

    // Can't access some functionality until project created or loaded.
    this->updateMenus();
    this->updateRecentProjects();

    // Setup progress dialog.
    this->progressDialog = new QProgressDialog(this);
    this->progressDialog->setMinimumDuration(500);
    this->progressDialog->setModal(true);
    this->progressDialog->setCancelButton(0);
    this->progressDialog->setMaximum(1);
    this->progressDialog->setValue(1);

    // Setup undo.
    this->ui->menuProject->addSeparator();
    this->ui->mainToolBar->addSeparator();

    QAction* undoAction = this->controller->getUndoController().createUndoAction(this, tr("Undo"));
    undoAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Z));
    undoAction->setIcon(QIcon(":/Media/Icons/Undo_16x.png"));
    this->ui->menuProject->addAction(undoAction);
    this->ui->mainToolBar->addAction(undoAction);

    QAction* redoAction = this->controller->getUndoController().createRedoAction(this, tr("Redo"));
    redoAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Y));
    redoAction->setIcon(QIcon(":/Media/Icons/Redo_16x.png"));
    this->ui->menuProject->addAction(redoAction);
    this->ui->mainToolBar->addAction(redoAction);
}

MainWindow::~MainWindow()
{
    delete this->ui;

    delete this->recordTreeWidget;
    delete this->recordFieldTableWidget;
    delete this->outputDockWidget;
    delete this->errorListDockWidget;

    delete this->aboutWindow;
    delete this->componentsWindow;
    delete this->customTypesWindow;
    delete this->fieldDefinitionsWindow;
    delete this->fieldValueWindow;
    delete this->newProjectWindow;
    delete this->recordWindow;
    delete this->findRecordWindow;
    delete this->projectOverviewWindow;
    delete this->userSettingsWindow;
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    // Check if we have unsaved changes.
    if (this->controller->getUndoController().isClean())
    {
        event->accept();
        return;
    }

    // Ask user whether they want to save their changes.
    QMessageBox::StandardButton result = QMessageBox::question(this,
                                                               tr("Tome"),
                                                               tr("Want to save your changes before exiting?"),
                                                               QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
                                                               QMessageBox::Cancel);
    if (result == QMessageBox::Yes)
    {
        this->controller->getProjectController().saveProject();
        event->accept();
    }
    else if (result == QMessageBox::No)
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::on_actionProject_Overview_triggered()
{
    if (!this->projectOverviewWindow)
    {
        this->projectOverviewWindow = new ProjectOverviewWindow(
                    this->controller->getProjectController(),
                    this->controller->getComponentsController(),
                    this->controller->getExportController(),
                    this->controller->getFieldDefinitionsController(),
                    this->controller->getImportController(),
                    this->controller->getRecordsController(),
                    this->controller->getTypesController(),
                    this);
    }

    this->showWindow(this->projectOverviewWindow);
}

void MainWindow::on_actionField_Definions_triggered()
{
    if (!this->fieldDefinitionsWindow)
    {
        this->fieldDefinitionsWindow = new FieldDefinitionsWindow(
                    this->controller->getFieldDefinitionsController(),
                    this->controller->getComponentsController(),
                    this->controller->getFacetsController(),
                    this->controller->getFindUsagesController(),
                    this->controller->getProjectController(),
                    this->controller->getRecordsController(),
                    this->controller->getTypesController(),
                    this->controller->getUndoController(),
                    this);

        connect(
                    this->fieldDefinitionsWindow,
                    SIGNAL(fieldChanged()),
                    SLOT(onFieldChanged())
                    );
    }

    this->showWindow(this->fieldDefinitionsWindow);
}

void MainWindow::on_actionManage_Components_triggered()
{
    if (!this->componentsWindow)
    {
        this->componentsWindow = new ComponentsWindow(
                    this->controller->getComponentsController(),
                    this->controller->getFieldDefinitionsController(),
                    this->controller->getUndoController(),
                    this);
    }

    this->showWindow(this->componentsWindow);
}

void MainWindow::on_actionManage_Custom_Types_triggered()
{
    if (!this->customTypesWindow)
    {
        this->customTypesWindow = new CustomTypesWindow(
                    this->controller->getTypesController(),
                    this->controller->getFacetsController(),
                    this->controller->getFieldDefinitionsController(),
                    this->controller->getFindUsagesController(),
                    this->controller->getRecordsController(),
                    this->controller->getUndoController(),
                    this);
    }

    this->showWindow(this->customTypesWindow);
}

void MainWindow::on_actionNew_Project_triggered()
{
    if (!this->newProjectWindow)
    {
        this->newProjectWindow = new NewProjectWindow(this);
    }

    int result = this->newProjectWindow->exec();

    if (result == QDialog::Accepted)
    {
        const QString& projectName = this->newProjectWindow->getProjectName();
        const QString& projectPath = this->newProjectWindow->getProjectPath();
        const RecordIdType::RecordIdType recordIdType = this->newProjectWindow->getProjectRecordIdType();

        // Check if we have unsaved changes.
        if (!this->controller->getUndoController().isClean())
        {
            // Ask user whether they want to save their changes.
            QMessageBox::StandardButton result = QMessageBox::question(this,
                                                                       tr("Tome"),
                                                                       tr("Want to save your changes before exiting?"),
                                                                       QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
                                                                       QMessageBox::Cancel);

            if (result == QMessageBox::Yes)
            {
                this->controller->getProjectController().saveProject();
            }
            else if (result == QMessageBox::Cancel)
            {
                return;
            }
        }

        try
        {
            this->controller->getProjectController().createProject(projectName, projectPath, recordIdType);
        }
        catch (std::runtime_error& e)
        {
            QMessageBox::critical(
                        this,
                        tr("Unable to create project"),
                        e.what(),
                        QMessageBox::Close,
                        QMessageBox::Close);
        }
    }
}

void MainWindow::on_actionOpen_Project_triggered()
{
    // Open file browser dialog.
    QString path = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);

    if( !this->controller->getSettingsController().getLastProjectPath().isEmpty())
    {
        path = this->controller->getSettingsController().getLastProjectPath();
    }

    const QString& projectFileName = QFileDialog::getOpenFileName(this,
                                                                  tr("Open Project"),
                                                                  path,
                                                                  "Tome Project Files (*.tproj)");

    if( !projectFileName.isEmpty() && QFile::exists( projectFileName ) )
    {
        QFileInfo info( projectFileName );

        this->controller->getSettingsController().setLastProjectPath( info.absolutePath() );
    }
    this->openProject(projectFileName);
}

void MainWindow::on_actionSave_Project_triggered()
{
    try
    {
        this->controller->getProjectController().saveProject();

        if (this->controller->getSettingsController().getRunIntegrityChecksOnSave())
        {
            this->on_actionRun_Integrity_Checks_triggered();
        }

        this->controller->getUndoController().setClean();
    }
    catch (std::runtime_error& e)
    {
        QMessageBox::critical(
                    this,
                    tr("Unable to create project"),
                    e.what(),
                    QMessageBox::Close,
                    QMessageBox::Close);
    }
}

void MainWindow::on_actionReload_Project_triggered()
{
    const QString fullProjectPath = this->controller->getProjectController().getFullProjectPath();
    this->openProject(fullProjectPath);
}

void MainWindow::on_actionNew_Record_triggered()
{
    if (!this->recordWindow)
    {
        this->recordWindow = new RecordWindow(this);
    }

    RecordsController& recordsController = this->controller->getRecordsController();

    // Set record id type.
    RecordIdType::RecordIdType recordIdType = this->controller->getProjectController().getProjectRecordIdType();
    this->recordWindow->setRecordIdType(recordIdType);
    this->recordWindow->setRecordId(QVariant());

    // Add fields.
    const FieldDefinitionList& fieldDefinitions =
            this->controller->getFieldDefinitionsController().getFieldDefinitions();
    const QVariantList recordIds = recordsController.getRecordIds();
    const ComponentList& componentDefinitions =
            this->controller->getComponentsController().getComponents();

    // When creating new records, automatically update record id to reflect display name.
    this->recordWindow->setRecordIdLocked(false);

    // Disallow all existing record ids.
    this->recordWindow->setDisallowedRecordIds(recordIds);

    // Set fields.
    this->recordWindow->setRecordFields(fieldDefinitions);

    // Set components.
    this->recordWindow->setRecordComponents(componentDefinitions);

    // Set record set names.
    const QStringList recordSetNames = recordsController.getRecordSetNames();
    this->recordWindow->setRecordSetNames(recordSetNames);
    this->recordWindow->setRecordSetName(recordSetNames.first());

    // Show window.
    int result = this->recordWindow->exec();

    if (result == QDialog::Accepted)
    {
        const QVariant& recordId = this->recordWindow->getRecordId();
        const QString& recordDisplayName = this->recordWindow->getRecordDisplayName();
        const QString& recordSetName = this->recordWindow->getRecordSetName();
        const QString& recordEditorIconFieldId = this->recordWindow->getRecordEditorIconFieldId();

        // Collect record fields.
        QStringList recordFieldIds;

        const QMap<QString, RecordFieldState::RecordFieldState> recordFields = this->recordWindow->getRecordFields();

        for (QMap<QString, RecordFieldState::RecordFieldState>::const_iterator it = recordFields.begin();
             it != recordFields.end();
             ++it)
        {
            const QString& fieldId = it.key();
            const RecordFieldState::RecordFieldState fieldState = it.value();

            if (fieldState == RecordFieldState::Enabled)
            {
                recordFieldIds << fieldId;
            }
        }

        // Update model.
        AddRecordCommand* command = new AddRecordCommand(recordsController,
                                                         recordId,
                                                         recordDisplayName,
                                                         recordEditorIconFieldId,
                                                         recordFieldIds,
                                                         recordSetName);
        this->controller->getUndoController().doCommand(command);
    }
}

void MainWindow::on_actionAdd_Child_triggered()
{
    const QVariant& parentId = this->recordTreeWidget->getSelectedRecordId();

    if (parentId.isNull())
    {
        return;
    }

    RecordsController& recordsController = this->controller->getRecordsController();

    // Get selected record.
    const Record& parentRecord = recordsController.getRecord(parentId);

    // Show window.
    if (!this->recordWindow)
    {
        this->recordWindow = new RecordWindow(this);
    }

    // Set record id type.
    RecordIdType::RecordIdType recordIdType = this->controller->getProjectController().getProjectRecordIdType();
    this->recordWindow->setRecordIdType(recordIdType);
    this->recordWindow->setRecordId(QVariant());

    // When creating new records, automatically update record id to reflect display name.
    this->recordWindow->setRecordIdLocked(false);

    // Disallow all existing record ids.
    const QVariantList recordIds = recordsController.getRecordIds();

    this->recordWindow->setDisallowedRecordIds(recordIds);

    // Set fields.
    const FieldDefinitionList& fieldDefinitions =
            this->controller->getFieldDefinitionsController().getFieldDefinitions();
    const ComponentList& componentDefinitions =
            this->controller->getComponentsController().getComponents();
    const RecordFieldValueMap fieldValues = recordsController.getRecordFieldValues(parentId);

    this->recordWindow->setRecordFields(fieldDefinitions, componentDefinitions, RecordFieldValueMap(), fieldValues);

    // Set record set.
    const QStringList recordSetNames = recordsController.getRecordSetNames();
    this->recordWindow->setRecordSetNames(recordSetNames);
    this->recordWindow->setRecordSetName(parentRecord.recordSetName);

    // Show window.
    int result = this->recordWindow->exec();

    if (result == QDialog::Accepted)
    {
        const QVariant recordId = this->recordWindow->getRecordId();
        const QString recordDisplayName = this->recordWindow->getRecordDisplayName();
        const QString recordSetName = this->recordWindow->getRecordSetName();
        const QString recordEditorIconFieldId = this->recordWindow->getRecordEditorIconFieldId();

        QStringList recordFieldIds;

        const QMap<QString, RecordFieldState::RecordFieldState> recordFields = this->recordWindow->getRecordFields();

        for (QMap<QString, RecordFieldState::RecordFieldState>::const_iterator it = recordFields.begin();
             it != recordFields.end();
             ++it)
        {
            const QString& fieldId = it.key();
            const RecordFieldState::RecordFieldState fieldState = it.value();

            if (fieldState == RecordFieldState::Enabled)
            {
                recordFieldIds << fieldId;
            }
        }

        // Update model.
        AddRecordCommand* command = new AddRecordCommand(recordsController,
                                                         recordId,
                                                         recordDisplayName,
                                                         recordEditorIconFieldId,
                                                         recordFieldIds,
                                                         recordSetName,
                                                         parentId);
        this->controller->getUndoController().doCommand(command);
    }
}

void MainWindow::on_actionEdit_Record_triggered()
{
    const QVariant& id = this->recordTreeWidget->getSelectedRecordId();

    if (id.isNull())
    {
        return;
    }

    RecordsController& recordsController = this->controller->getRecordsController();

    // Get selected record.
    const Record& record = recordsController.getRecord(id);

    // Check if read-only.
    if (record.readOnly && !this->controller->getProjectController().getProjectIgnoreReadOnly())
    {
        this->showReadOnlyMessage(record.id);
        return;
    }

    // Show window.
    if (!this->recordWindow)
    {
        this->recordWindow = new RecordWindow(this);
    }

    // Set record id and name.
    this->recordWindow->setRecordIdType(this->controller->getProjectController().getProjectRecordIdType());
    this->recordWindow->setRecordId(record.id);
    this->recordWindow->setRecordIdLocked(true);
    this->recordWindow->setRecordDisplayName(record.displayName);

    // Disallow all other record ids.
    QVariantList recordIds = recordsController.getRecordIds();
    recordIds.removeOne(record.id);

    this->recordWindow->setDisallowedRecordIds(recordIds);

    // Set fields.
    const FieldDefinitionList& fieldDefinitions =
            this->controller->getFieldDefinitionsController().getFieldDefinitions();
    const RecordFieldValueMap inheritedFieldValues =
            recordsController.getInheritedFieldValues(record.id);
    const ComponentList& componentDefinitions =
            this->controller->getComponentsController().getComponents();

    this->recordWindow->setRecordFields(fieldDefinitions, componentDefinitions, record.fieldValues, inheritedFieldValues);
    this->recordWindow->setRecordEditorIconFieldId(record.editorIconFieldId);

    // Set record set.
    this->recordWindow->setRecordSetNames(recordsController.getRecordSetNames());
    this->recordWindow->setRecordSetName(record.recordSetName);

    int result = this->recordWindow->exec();

    if (result == QDialog::Accepted)
    {
        const QVariant recordId = this->recordWindow->getRecordId();
        const QString recordDisplayName = this->recordWindow->getRecordDisplayName();
        const QString recordSetName = this->recordWindow->getRecordSetName();
        const QString recordEditorIconFieldId = this->recordWindow->getRecordEditorIconFieldId();

        QStringList recordFieldIds;

        const QMap<QString, RecordFieldState::RecordFieldState> recordFields = this->recordWindow->getRecordFields();

        for (QMap<QString, RecordFieldState::RecordFieldState>::const_iterator it = recordFields.begin();
             it != recordFields.end();
             ++it)
        {
            const QString& fieldId = it.key();
            const RecordFieldState::RecordFieldState fieldState = it.value();

            if (fieldState == RecordFieldState::Enabled)
            {
                recordFieldIds << fieldId;
            }
        }

        // Update model.
        UpdateRecordCommand* command = new UpdateRecordCommand(recordsController,
                                                               record.id,
                                                               recordId,
                                                               recordDisplayName,
                                                               recordEditorIconFieldId,
                                                               recordFieldIds,
                                                               recordSetName);
        this->controller->getUndoController().doCommand(command);
    }
}

void MainWindow::on_actionDuplicate_Record_triggered()
{
    if (!this->duplicateRecordWindow)
    {
        this->duplicateRecordWindow = new DuplicateRecordWindow(this);
    }

    const QVariant& recordId = this->recordTreeWidget->getSelectedRecordId();
    const QVariantList recordIds = this->controller->getRecordsController().getRecordIds();

    if (recordId.isNull())
    {
        return;
    }

    // Check record id type.
    RecordIdType::RecordIdType recordIdType = this->controller->getProjectController().getProjectRecordIdType();
    DuplicateRecordCommand* command;

    if (recordIdType == RecordIdType::String)
    {
        // Disallow all existing record ids.
        this->duplicateRecordWindow->setRecordId(recordId.toString());

        QStringList recordStringIds;
        for (QVariant v : recordIds)
        {
            recordStringIds << v.toString();
        }

        this->duplicateRecordWindow->setDisallowedRecordIds(recordStringIds);

        // Show window.
        int result = this->duplicateRecordWindow->exec();

        if (result != QDialog::Accepted)
        {
            return;
        }

        const QString& newRecordId = this->duplicateRecordWindow->getRecordId();


        command = new DuplicateRecordCommand(this->controller->getRecordsController(),
                                                                     recordId,
                                                                     newRecordId);
    }
    else
    {
        command = new DuplicateRecordCommand(this->controller->getRecordsController(),
                                                                     recordId,
                                                                     QVariant());
    }

    // Update model.
    this->controller->getUndoController().doCommand(command);
}

void MainWindow::on_actionRevert_Record_triggered()
{
    // Get records to revert.
    const QVariantList recordIds = this->recordTreeWidget->getSelectedRecordIds();

    if (recordIds.empty())
    {
        return;
    }

    if (recordIds.count() > 1)
    {
        if (!this->controller->getRecordsController().haveTheSameParent(recordIds))
        {
            QMessageBox::information(
                        this,
                        tr("Revert Records"),
                        tr("Cannot revert multiple records with different parents."),
                        QMessageBox::Close,
                        QMessageBox::Close);
            return;
        }

        // Show question.
        const QString& question = tr("Are you sure you want to revert %1 records to their original state?").arg(recordIds.count());

        int answer = QMessageBox::question(
                    this,
                    tr("Revert Records"),
                    question,
                    QMessageBox::Yes,
                    QMessageBox::No);

        if (answer != QMessageBox::Yes)
        {
            return;
        }

        for (int i = 0; i < recordIds.count(); ++i)
        {
            // Check if read-only.
            const QVariant recordId = recordIds[i];
            const Record& record = this->controller->getRecordsController().getRecord(recordId);

            if (record.readOnly && !this->controller->getProjectController().getProjectIgnoreReadOnly())
            {
                this->showReadOnlyMessage(recordId);
                continue;
            }

            // Revert record.
            RevertRecordCommand* command = new RevertRecordCommand(
                        this->controller->getRecordsController(),
                        recordId);
            this->controller->getUndoController().doCommand(command);
        }
    }
    else
    {
        // Check if read-only.
        const QVariant recordId = recordIds[0];
        const Record& record = this->controller->getRecordsController().getRecord(recordId);

        if (record.readOnly && !this->controller->getProjectController().getProjectIgnoreReadOnly())
        {
            this->showReadOnlyMessage(recordId);
            return;
        }

        // Show question.
        const QString& question = tr("Are you sure you want to revert %1 to its original state?")
                .arg(record.displayName);

        int answer = QMessageBox::question(
                    this,
                    tr("Revert Record"),
                    question,
                    QMessageBox::Yes,
                    QMessageBox::No);

        if (answer != QMessageBox::Yes)
        {
            return;
        }

        // Revert record.
        RevertRecordCommand* command = new RevertRecordCommand(
                    this->controller->getRecordsController(),
                    recordId);
        this->controller->getUndoController().doCommand(command);
    }
}

void MainWindow::on_actionRemove_Record_triggered()
{
    QVariantList recordIds = this->recordTreeWidget->getSelectedRecordIds();

    if (!this->controller->getRecordsController().haveTheSameParent(recordIds))
    {
        QMessageBox::information(
                    this,
                    tr("Remove Records"),
                    tr("Cannot remove multiple records with different parents."),
                    QMessageBox::Close,
                    QMessageBox::Close);
        return;
    }

    for (int i = 0; i < recordIds.count(); ++i)
    {
        const QVariant recordId = recordIds[i];

        // Update progress bar.
        this->onProgressChanged(tr("Removing Records"), recordId.toString(), i, recordIds.count());

        // Check if read-only.
        const Record& record = this->controller->getRecordsController().getRecord(recordId);

        if (record.readOnly && !this->controller->getProjectController().getProjectIgnoreReadOnly())
        {
            this->showReadOnlyMessage(recordId);
            continue;
        }

        // Update model.
        RemoveRecordCommand* command = new RemoveRecordCommand(
                    this->controller->getRecordsController(),
                    this->controller->getFieldDefinitionsController(),
                    this->controller->getTypesController(),
                    recordId);
        this->controller->getUndoController().doCommand(command);
    }

    // Clear progress bar.
    this->onProgressChanged(tr("Removing Records"), QString(), 1, 1);
}

void MainWindow::on_actionFindRecord_triggered()
{
    // Show window.
    if (!this->findRecordWindow)
    {
        this->findRecordWindow = new FindRecordWindow();
    }

    int result = this->findRecordWindow->exec();

    if (result != QDialog::Accepted)
    {
        return;
    }

    // Find record.
    QString searchPattern = this->findRecordWindow->getSearchPattern();
    this->controller->getFindRecordController().findRecord(searchPattern);
}

void MainWindow::on_actionFind_Usages_triggered()
{
    // Find usages.
    const QVariant& recordId = this->recordTreeWidget->getSelectedRecordId();

    if (recordId.isNull())
    {
        return;
    }

    this->controller->getFindUsagesController().findUsagesOfRecord(recordId);
}


void MainWindow::on_actionRun_Integrity_Checks_triggered()
{
    // Run tasks.
    this->messages = this->controller->getTasksController().runAllTasks();

    // Update view.
    this->showWindow(this->errorListDockWidget);
    this->refreshErrorList();
}

void MainWindow::on_actionUser_Settings_triggered()
{
    if (!this->userSettingsWindow)
    {
        this->userSettingsWindow = new UserSettingsWindow(this->controller->getSettingsController(), this);
    }

    int result = this->userSettingsWindow->exec();

    if (result != QDialog::Accepted)
    {
        return;
    }

    // Save settings.
    SettingsController& settingsController = this->controller->getSettingsController();
    settingsController.setRunIntegrityChecksOnLoad(this->userSettingsWindow->getRunIntegrityChecksOnLoad());
    settingsController.setRunIntegrityChecksOnSave(this->userSettingsWindow->getRunIntegrityChecksOnSave());
    settingsController.setShowDescriptionColumnInsteadOfFieldTooltips(this->userSettingsWindow->getShowDescriptionColumnInsteadOfFieldTooltips());
    settingsController.setExpandRecordTreeOnRefresh(this->userSettingsWindow->getExpandRecordTreeOnRefresh());
    settingsController.setShowComponentNamesInRecordTable(this->userSettingsWindow->getShowComponentNamesInRecordTable());

    // Refresh view with updated settings.
    this->refreshRecordTable();
}

void MainWindow::on_actionAbout_triggered()
{
    if (!this->aboutWindow)
    {
        this->aboutWindow = new AboutWindow(this);
    }

    this->showWindow(this->aboutWindow);
}

void MainWindow::on_actionManual_triggered()
{
    QDesktopServices::openUrl(QUrl("https://github.com/npruehs/tome-editor/wiki/Introduction"));
}

void MainWindow::on_actionReport_a_Bug_triggered()
{
    QProcess *process = new QProcess();
    process->start("TomeIssueReporter.exe");
}

void MainWindow::on_actionReleases_triggered()
{
    QDesktopServices::openUrl(QUrl("https://github.com/npruehs/tome-editor/releases"));
}

void MainWindow::on_actionRoadmap_triggered()
{
    QDesktopServices::openUrl(QUrl("https://github.com/npruehs/tome-editor/milestones?direction=asc&sort=due_date&state=open"));
}

void MainWindow::on_actionOutput_triggered()
{
    this->showWindow(this->outputDockWidget);
}

void MainWindow::on_actionError_List_triggered()
{
    this->showWindow(this->errorListDockWidget);
}

void MainWindow::on_actionNavigate_Backward_triggered()
{
    this->recordTreeWidget->navigateBackward();
}

void MainWindow::on_actionNavigate_Forward_triggered()
{
    this->recordTreeWidget->navigateForward();
}

void MainWindow::exportRecords(QAction* exportAction)
{
    // Get export template.
    QString exportTemplateName = exportAction->text();
    const RecordExportTemplate& exportTemplate =
            this->controller->getExportController().getRecordExportTemplate(exportTemplateName);

    // Build export file name suggestion.
    const QString suggestedFileName = this->controller->getProjectController().getProjectName() + exportTemplate.fileExtension;
    const QString suggestedFilePath = combinePaths(this->controller->getProjectController().getProjectPath(), suggestedFileName);
    const QString filter = exportTemplateName + " (*" + exportTemplate.fileExtension + ")";

    // Show file dialog.
    QString filePath = QFileDialog::getSaveFileName(this,
                                                    tr("Export Records"),
                                                    suggestedFilePath,
                                                    filter);

    if (filePath.isEmpty())
    {
        return;
    }

    // Export records.
    try
    {
        this->controller->getExportController().exportRecords(exportTemplate, filePath);
    }
    catch (std::runtime_error& e)
    {
        QMessageBox::critical(
                    this,
                    tr("Unable to export records"),
                    e.what(),
                    QMessageBox::Close,
                    QMessageBox::Close);
    }
}

void MainWindow::importRecords(QAction* importAction)
{
    // Get import template.
    QString importTemplateName = importAction->text();
    const RecordTableImportTemplate& importTemplate =
            this->controller->getImportController().getRecordTableImportTemplate(importTemplateName);

    // Request import source.
    QString sourceUrl;

    switch (importTemplate.sourceType)
    {
        case TableType::Csv:
            sourceUrl = QFileDialog::getOpenFileName(this,
                                                     tr("Import Records"),
                                                     this->controller->getProjectController().getProjectPath(),
                                                     "Comma-Separated Values (*.csv)");
            break;

        case TableType::GoogleSheets:
            sourceUrl = QInputDialog::getText(this,
                                              tr("Import Records"),
                                              tr("Google Sheet ID:"),
                                              QLineEdit::Normal,
                                              "1dyN-0I5I9DZGrURGfpOBElq8Ng4tyGtuZp4T3OBDjys");
            break;

        case TableType::Xlsx:
            sourceUrl = QFileDialog::getOpenFileName(this,
                                                     tr("Import Records"),
                                                     this->controller->getProjectController().getProjectPath(),
                                                     "Excel Workbook (*.xlsx)");
            break;

        default:
            QMessageBox::critical(
                        this,
                        tr("Unable to import records"),
                        tr("Unknown source type."),
                        QMessageBox::Close,
                        QMessageBox::Close);
    }

    if (sourceUrl.isEmpty())
    {
        return;
    }

    // Import records.
    try
    {
        this->controller->getImportController().importRecords(importTemplate, sourceUrl);
    }
    catch (std::runtime_error& e)
    {
        QMessageBox::critical(
                    this,
                    tr("Unable to import records"),
                    e.what(),
                    QMessageBox::Close,
                    QMessageBox::Close);
    }
}

void MainWindow::onExportTemplatesChanged()
{
    this->refreshExportMenu();
}

void MainWindow::openRecentProject(QAction* recentProjectAction)
{
    QString path = recentProjectAction->text();
    this->openProject(path);
}

void MainWindow::revertFieldValue()
{
    // Get record to revert field of.
    QVariant recordId = this->recordTreeWidget->getSelectedRecordId();

    // Get field to revert.
    QModelIndexList selectedIndexes = this->recordFieldTableWidget->selectionModel()->selectedRows(0);

    if (selectedIndexes.empty())
    {
        return;
    }

    const RecordFieldValueMap fieldValues =
            this->controller->getRecordsController().getRecordFieldValues(recordId);
    const QString fieldId = fieldValues.keys()[selectedIndexes.first().row()];

    // Get inherited field value.
    QVariant valueToRevertTo = this->controller->getRecordsController().getInheritedFieldValue(recordId, fieldId);

    if (valueToRevertTo == QVariant())
    {
        // Get default field value.
        const FieldDefinition& field =
                this->controller->getFieldDefinitionsController().getFieldDefinition(fieldId);

        valueToRevertTo = field.defaultValue;
    }

    // Update view.
    if (this->fieldValueWindow != 0)
    {
        this->fieldValueWindow->setFieldValue(valueToRevertTo);
    }
}

void MainWindow::searchResultChanged(const QString& title, const SearchResultList results)
{
    // Update view.
    this->showWindow(this->searchResultsDockWidget);
    this->searchResultsDockWidget->showResults(title, results);
}

void MainWindow::tableWidgetDoubleClicked(const QModelIndex &index)
{
    QVariantList ids = this->recordTreeWidget->getSelectedRecordIds();
    QVariant id = ids.first();

    const RecordFieldValueMap fieldValues =
            this->controller->getRecordsController().getRecordFieldValues(id);

    // Get current field data.
    const QString fieldId = index.data(Qt::UserRole).toString();
    const QVariant fieldValue = fieldValues[fieldId];

    const FieldDefinition& field =
            this->controller->getFieldDefinitionsController().getFieldDefinition(fieldId);

    // Prepare window.
    if (!this->fieldValueWindow)
    {
        this->fieldValueWindow = new FieldValueWindow
                (this->controller->getFacetsController(),
                 this->controller->getProjectController(),
                 this->controller->getRecordsController(),
                 this->controller->getTypesController(),
                 this);

        connect(
                    this->fieldValueWindow,
                    SIGNAL(revert()),
                    SLOT(revertFieldValue())
                    );
    }

    // Update view.
    this->fieldValueWindow->setFieldCount(ids.count());
    this->fieldValueWindow->setFieldDisplayName(field.displayName);
    this->fieldValueWindow->setFieldDescription(field.description);
    this->fieldValueWindow->setFieldType(field.fieldType);
    this->fieldValueWindow->setFieldValue(fieldValue);

    // Apply facets.
    QString facetsDescription;

    if (this->controller->getTypesController().isCustomType(field.fieldType))
    {
        const CustomType& customType = this->controller->getTypesController().getCustomType(field.fieldType);

        if (customType.isDerivedType())
        {
            QString baseType = customType.getBaseType();
            QList<Facet*> facets = this->controller->getFacetsController().getFacets(baseType);

            for (int i = 0; i < facets.count(); ++i)
            {
                Facet* facet = facets[i];
                QString facetKey = facet->getKey();

                if (customType.constrainingFacets.contains(facetKey))
                {
                    QVariant facetValue = customType.constrainingFacets[facetKey];
                    facetsDescription += facet->getDescriptionForValue(facetValue);
                    facetsDescription += " ";
                }
            }

            this->fieldValueWindow->setFieldDescription(field.description + " " + facetsDescription);
        }
    }

    // Show window.
    int result = this->fieldValueWindow->exec();

    if (result == QDialog::Accepted)
    {
        QVariant fieldValue = this->fieldValueWindow->getFieldValue();

        // Update model of all selected records.
        for (int i = 0; i < ids.count(); ++i)
        {
            UpdateRecordFieldValueCommand* command =
                    new UpdateRecordFieldValueCommand(this->controller->getRecordsController(), ids[i], fieldId, fieldValue);
            this->controller->getUndoController().doCommand(command);
        }

        // Update view.
        this->recordFieldTableWidget->updateFieldValue(index.row());
    }
}

void MainWindow::treeWidgetDoubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index)
    this->on_actionEdit_Record_triggered();
}

void MainWindow::treeWidgetRecordReparented(const QVariant& recordId, const QVariant& newParentId)
{
    // Check if allowed.
    if (recordId == newParentId ||
            this->controller->getRecordsController().isAncestorOf(recordId, newParentId))
    {
        return;
    }

    // Check if read-only.
    const Record& record = this->controller->getRecordsController().getRecord(recordId);

    if (record.readOnly && !this->controller->getProjectController().getProjectIgnoreReadOnly())
    {
        this->showReadOnlyMessage(recordId);
        return;
    }

    // Update model.
    ReparentRecordCommand* command = new ReparentRecordCommand(
                this->controller->getRecordsController(),
                recordId,
                newParentId);
    this->controller->getUndoController().doCommand(command);
}

void MainWindow::treeWidgetSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected)
{
    Q_UNUSED(selected)
    Q_UNUSED(deselected)

    // Update field table.
    this->refreshRecordTable();

    // Update actions.
    bool anyRecordSelected = !selected.isEmpty();
    this->ui->actionAdd_Child->setEnabled(anyRecordSelected);
    this->ui->actionDuplicate_Record->setEnabled(anyRecordSelected);
    this->ui->actionEdit_Record->setEnabled(anyRecordSelected);
    this->ui->actionFind_Usages->setEnabled(anyRecordSelected);
    this->ui->actionRemove_Record->setEnabled(anyRecordSelected);
    this->ui->actionRevert_Record->setEnabled(anyRecordSelected);
}

QString MainWindow::getReadOnlyMessage(const QVariant& recordId)
{
    return tr("The record %1 has been marked as read-only. You cannot edit, reparent or remove read-only records.")
            .arg(recordId.toString());
}

void MainWindow::openProject(QString path)
{
    if (path.isEmpty())
    {
        return;
    }

    // Check if we have unsaved changes.
    if (!this->controller->getUndoController().isClean())
    {
        // Ask user whether they want to save their changes.
        QMessageBox::StandardButton result = QMessageBox::question(this,
                                                                   tr("Tome"),
                                                                   tr("Want to save your changes before exiting?"),
                                                                   QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
                                                                   QMessageBox::Cancel);

        if (result == QMessageBox::Yes)
        {
            this->controller->getProjectController().saveProject();
        }
        else if (result == QMessageBox::Cancel)
        {
            return;
        }
    }

    try
    {
        this->controller->getProjectController().openProject(path);
    }
    catch (std::runtime_error& e)
    {
        // Remove from recent projects.
        this->controller->getSettingsController().removeRecentProject(path);
        this->updateRecentProjects();

        QMessageBox::critical(
                    this,
                    tr("Unable to open project"),
                    e.what(),
                    QMessageBox::Close,
                    QMessageBox::Close);
    }
}

void MainWindow::onFieldChanged()
{
    this->refreshRecordTable();
}

void MainWindow::onImportError(const QString& error)
{
    QMessageBox::critical(
                this,
                tr("Unable to import records"),
                error,
                QMessageBox::Close,
                QMessageBox::Close);
}

void MainWindow::onImportFinished()
{
    this->refreshRecordTreeAfterReparent = true;
    this->refreshRecordTree();
}

void MainWindow::onImportStarted()
{
    this->refreshRecordTreeAfterReparent = false;
}

void MainWindow::onImportTemplatesChanged()
{
    this->refreshImportMenu();
}

void MainWindow::onFileLinkActivated(const QString& filePath)
{
    showFileInExplorerOrFinder(filePath);
}

void MainWindow::onProgressChanged(const QString title, const QString text, const int currentValue, const int maximumValue)
{
    // Check if finished.
    if (currentValue >= maximumValue)
    {
        this->progressDialog->reset();
        return;
    }

    // Setup dialog.
    this->progressDialog->setWindowTitle(title);
    this->progressDialog->setLabelText(text);
    this->progressDialog->setMaximum(maximumValue);
    this->progressDialog->setValue(currentValue);

    // Set dialog width, accounting for dialog buttons in title bar.
    QFontMetrics fontMetrics(this->progressDialog->font());
    int titleWidth = fontMetrics.width(title) + 150;
    int textWidth = fontMetrics.width(text);
    int dialogWidth = titleWidth > textWidth ? titleWidth : textWidth;
    this->progressDialog->setFixedWidth(dialogWidth);
}

void MainWindow::onProjectChanged(QSharedPointer<Project> project)
{
    Q_UNUSED(project)

    // Enable project-specific buttons.
    this->updateMenus();

    // Setup tree view.
    this->recordTreeWidget->clear();
    this->recordFieldTableWidget->setRowCount(0);

    this->refreshRecordTree();

    // Setup record exports.
    this->refreshExportMenu();

    // Setup record imports.
    this->refreshImportMenu();

    // Update title.
    this->updateWindowTitle();

    // Update recent projects.
    this->updateRecentProjects();

    // Run integrity checks.
    if (this->controller->getSettingsController().getRunIntegrityChecksOnLoad())
    {
        this->on_actionRun_Integrity_Checks_triggered();
    }
}

void MainWindow::onRecordAdded(const QVariant& recordId, const QString& recordDisplayName, const QVariant& parentId)
{
    // Update view.
    this->recordTreeWidget->addRecord(recordId, recordDisplayName, parentId);
    this->refreshRecordTable();
}

void MainWindow::onRecordFieldsChanged(const QVariant& recordId)
{
    if (this->recordTreeWidget->getSelectedRecordId() == recordId)
    {
        this->refreshRecordTable();
    }
}

void MainWindow::onRecordRemoved(const QVariant& recordId)
{
    this->recordTreeWidget->removeRecord(recordId);
}

void MainWindow::onRecordReparented(const QVariant& recordId, const QVariant& oldParentId, const QVariant& newParentId)
{
    Q_UNUSED(oldParentId)
    Q_UNUSED(newParentId)

    if (!this->refreshRecordTreeAfterReparent)
    {
        return;
    }

    // Update view.
    this->refreshRecordTree();
    this->recordTreeWidget->selectRecord(recordId, true);
}

void MainWindow::onRecordSetsChanged()
{
    this->refreshRecordTree();
}

void MainWindow::onRecordUpdated(const QVariant& oldId,
                                 const QString& oldDisplayName,
                                 const QString& oldEditorIconFieldId,
                                 const QVariant& newId,
                                 const QString& newDisplayName,
                                 const QString& newEditorIconFieldId)
{
    Q_UNUSED(oldDisplayName)

    // Update view.
    this->recordTreeWidget->updateRecord(oldId, oldDisplayName, oldEditorIconFieldId, newId, newDisplayName, newEditorIconFieldId);
    this->refreshRecordTable();
}

void MainWindow::onRecordLinkActivated(const QString& recordId)
{
    this->recordTreeWidget->selectRecord(recordId, true);
}

void MainWindow::onUndoStackChanged(bool clean)
{
    Q_UNUSED(clean)

    this->updateWindowTitle();
}

void MainWindow::refreshErrorList()
{
    this->errorListDockWidget->showMessages(this->messages);
}

void MainWindow::refreshExportMenu()
{
    this->ui->menuExport->clear();

    const RecordExportTemplateList& recordExportTemplateList =
            this->controller->getExportController().getRecordExportTemplates();

    for (RecordExportTemplateList::const_iterator it = recordExportTemplateList.begin();
         it != recordExportTemplateList.end();
         ++it)
    {
        QAction* exportAction = new QAction(it->name, this);
        this->ui->menuExport->addAction(exportAction);
    }
}

void MainWindow::refreshImportMenu()
{
    this->ui->menuImport->clear();

    const RecordTableImportTemplateList& recordTableImportTemplateList =
            this->controller->getImportController().getRecordTableImportTemplates();

    for (RecordTableImportTemplateList::const_iterator it = recordTableImportTemplateList.begin();
         it != recordTableImportTemplateList.end();
         ++it)
    {
        QAction* importAction = new QAction(it->name, this);
        this->ui->menuImport->addAction(importAction);
    }
}

void MainWindow::refreshRecordTree()
{
    this->recordTreeWidget->setRecords(this->controller->getRecordsController().getRecords());
}

void MainWindow::refreshRecordTable()
{
    const QVariant& id = this->recordTreeWidget->getSelectedRecordId();

    if (id.isNull() || !this->controller->getRecordsController().hasRecord(id))
    {
        // Clear table.
        this->recordFieldTableWidget->setRowCount(0);
        return;
    }

    // Update table.
    this->recordFieldTableWidget->setDescriptionColumnEnabled(
                this->controller->getSettingsController().getShowDescriptionColumnInsteadOfFieldTooltips());
    this->recordFieldTableWidget->setShowComponentNames(
                this->controller->getSettingsController().getShowComponentNamesInRecordTable());

    this->recordFieldTableWidget->setRecord(id);

    // Check if read-only.
    const Record& record = this->controller->getRecordsController().getRecord(id);

    if (record.readOnly && !this->controller->getProjectController().getProjectIgnoreReadOnly())
    {
        this->recordFieldTableWidget->setEnabled(false);
        this->recordFieldTableWidget->setToolTip(this->getReadOnlyMessage(id));
    }
    else
    {
        this->recordFieldTableWidget->setEnabled(true);
        this->recordFieldTableWidget->setToolTip(QString());
    }
}

void MainWindow::showReadOnlyMessage(const QVariant& recordId)
{
    QMessageBox::information(
                this,
                tr("Record is read-only"),
                this->getReadOnlyMessage(recordId),
                QMessageBox::Close,
                QMessageBox::Close);
}

void MainWindow::showWindow(QWidget* widget)
{
    widget->show();
    widget->raise();
    widget->activateWindow();

    // Check if dock widget.
    QDockWidget* dockWidget = qobject_cast<QDockWidget*>(widget);
    if (dockWidget != NULL)
    {
        // Tabify dock widget.
        for (int i = 0; i < this->dockWidgets.count(); ++i)
        {
            QDockWidget* existingDockWidget = this->dockWidgets[i];

            if (dockWidget != existingDockWidget && existingDockWidget->isVisible())
            {
                this->tabifyDockWidget(existingDockWidget, dockWidget);
            }
        }
    }
}

void MainWindow::updateMenus()
{
    bool projectLoaded = this->controller->getProjectController().isProjectLoaded();
    bool anyRecordSelected = this->recordTreeWidget->getSelectedRecordItem() != nullptr;

    // Update actions.
    this->ui->actionSave_Project->setEnabled(projectLoaded);

    this->ui->actionProject_Overview->setEnabled(projectLoaded);

    this->ui->actionField_Definions->setEnabled(projectLoaded);
    this->ui->actionManage_Components->setEnabled(projectLoaded);
    this->ui->actionManage_Custom_Types->setEnabled(projectLoaded);

    this->ui->actionNew_Record->setEnabled(projectLoaded);
    this->ui->actionFindRecord->setEnabled(projectLoaded);

    this->ui->actionAdd_Child->setEnabled(projectLoaded && anyRecordSelected);
    this->ui->actionEdit_Record->setEnabled(projectLoaded && anyRecordSelected);
    this->ui->actionDuplicate_Record->setEnabled(projectLoaded && anyRecordSelected);
    this->ui->actionRevert_Record->setEnabled(projectLoaded && anyRecordSelected);
    this->ui->actionRemove_Record->setEnabled(projectLoaded && anyRecordSelected);
    this->ui->actionFind_Usages->setEnabled(projectLoaded && anyRecordSelected);

    this->ui->actionRun_Integrity_Checks->setEnabled(projectLoaded);
}

void MainWindow::updateRecentProjects()
{
    // Clear menu.
    this->ui->menuRecent_Projects->clear();

    // Add recent projects.
    const QStringList& recentProjects = this->controller->getSettingsController().getRecentProjects();
    for (int i = 0; i < recentProjects.size(); ++i)
    {
        QString path = recentProjects.at(i);
        QAction* action = new QAction(path, this);
        this->ui->menuRecent_Projects->addAction(action);
    }
}

void MainWindow::updateWindowTitle()
{
    // Get application version.
    QString windowTitle = "Tome " + QApplication::instance()->applicationVersion();

    if (this->controller->getProjectController().isProjectLoaded())
    {
        // Add project name.
        windowTitle += " - " + this->controller->getProjectController().getFullProjectPath();
    }

    if (!this->controller->getUndoController().isClean())
    {
        windowTitle += "*";
    }

    this->setWindowTitle(windowTitle);
}
