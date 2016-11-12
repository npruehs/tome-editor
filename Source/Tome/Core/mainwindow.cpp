#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <stdexcept>

#include <QDesktopServices>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
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
#include "../Features/Projects/Model/project.h"
#include "../Features/Projects/Controller/projectserializer.h"
#include "../Features/Projects/View/newprojectwindow.h"
#include "../Features/Projects/View/projectoverviewwindow.h"
#include "../Features/Records/Controller/recordscontroller.h"
#include "../Features/Records/Controller/recordsetserializer.h"
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
    progressDialog(0)
{
    ui->setupUi(this);

    // Add record tree.
    this->recordTreeWidget = new RecordTreeWidget(this->controller->getRecordsController(), this->controller->getSettingsController());
    this->ui->splitter->addWidget(this->recordTreeWidget);

    // Add record table.
    this->recordFieldTableWidget = new RecordFieldsTableWidget(
                this->controller->getFieldDefinitionsController(),
                this->controller->getRecordsController(),
                this->controller->getTypesController());

    this->ui->splitter->addWidget(this->recordFieldTableWidget);

    // Add search results.
    this->searchResultsDockWidget = new SearchResultsDockWidget(this);
    this->addDockWidget(Qt::BottomDockWidgetArea, this->searchResultsDockWidget, Qt::Vertical);

    // Add error list.
    this->errorListDockWidget = new ErrorListDockWidget(this);
    this->addDockWidget(Qt::BottomDockWidgetArea, this->errorListDockWidget, Qt::Vertical);

    // Add log window.
    this->outputDockWidget = new OutputDockWidget(this);
    this->outputDockWidget->init();
    this->addDockWidget(Qt::BottomDockWidgetArea, this->outputDockWidget, Qt::Vertical);

    // Hide most dock widgets until required.
    this->searchResultsDockWidget->close();
    this->errorListDockWidget->close();

    // Connect signals.
    connect(
                this->controller,
                SIGNAL(projectChanged(QSharedPointer<Tome::Project>)),
                SLOT(onProjectChanged(QSharedPointer<Tome::Project>))
                );

    connect(
                &this->controller->getRecordsController(),
                SIGNAL(recordFieldsChanged(const QString&)),
                SLOT(onRecordFieldsChanged(const QString&))
                );

    connect(
                &this->controller->getRecordsController(),
                SIGNAL(recordSetsChanged()),
                SLOT(onRecordSetsChanged())
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
                SIGNAL(recordReparented(const QString&, const QString&)),
                SLOT(treeWidgetRecordReparented(const QString&, const QString&))
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
                this->controller,
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
                SIGNAL(recordLinkActivated(const QString&)),
                SLOT(onRecordLinkActivated(const QString&))
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

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::on_actionProject_Overview_triggered()
{
    if (!this->projectOverviewWindow)
    {
        this->projectOverviewWindow = new ProjectOverviewWindow(
                    this->controller,
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
                    this->controller->getRecordsController(),
                    this->controller->getTypesController(),
                    this->controller->getFindUsagesController(),
                    this->controller->getFacetsController(),
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

        try
        {
            this->controller->createProject(projectName, projectPath);
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
    const QString& projectFileName = QFileDialog::getOpenFileName(this,
                                                                  tr("Open Project"),
                                                                  QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
                                                                  "Tome Project Files (*.tproj)");

    this->openProject(projectFileName);
}

void MainWindow::on_actionSave_Project_triggered()
{
    try
    {
        this->controller->saveProject();

        if (this->controller->getSettingsController().getRunIntegrityChecksOnSave())
        {
            this->on_actionRun_Integrity_Checks_triggered();
        }
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

void MainWindow::on_actionNew_Record_triggered()
{
    if (!this->recordWindow)
    {
        this->recordWindow = new RecordWindow(this);
    }

    RecordsController& recordsController = this->controller->getRecordsController();

    // Add fields.
    const FieldDefinitionList& fieldDefinitions =
            this->controller->getFieldDefinitionsController().getFieldDefinitions();
    const QStringList recordIds = recordsController.getRecordIds();
    const ComponentList& componentDefinitions =
            this->controller->getComponentsController().getComponents();

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
        const QString& recordId = this->recordWindow->getRecordId();
        const QString& recordDisplayName = this->recordWindow->getRecordDisplayName();
        const QString& recordSetName = this->recordWindow->getRecordSetName();

        // Update model.
        recordsController.addRecord(recordId, recordDisplayName, recordSetName);

        // Update view.
        this->recordTreeWidget->addRecord(recordId, recordDisplayName);

        // Add record fields.
        const QMap<QString, RecordFieldState::RecordFieldState> recordFields = this->recordWindow->getRecordFields();

        for (QMap<QString, RecordFieldState::RecordFieldState>::const_iterator it = recordFields.begin();
             it != recordFields.end();
             ++it)
        {
            const QString& fieldId = it.key();
            const RecordFieldState::RecordFieldState fieldState = it.value();

            if (fieldState == RecordFieldState::Enabled)
            {
                this->addRecordField(fieldId);
            }
        }

        // Update view.
        this->refreshRecordTable();
        this->recordTreeWidget->updateRecordItem();
    }
}

void MainWindow::on_actionEdit_Record_triggered()
{
    const QString& id = this->recordTreeWidget->getSelectedRecordId();

    if (id.isEmpty())
    {
        return;
    }

    RecordsController& recordsController = this->controller->getRecordsController();

    // Get selected record.
    const Record& record = recordsController.getRecord(id);

    // Check if read-only.
    if (record.readOnly && !this->controller->getProjectIgnoreReadOnly())
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
    this->recordWindow->setRecordId(record.id);
    this->recordWindow->setRecordDisplayName(record.displayName);

    // Disallow all other record ids.
    QStringList recordIds = recordsController.getRecordIds();
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

    // Set record set.
    this->recordWindow->setRecordSetNames(recordsController.getRecordSetNames());
    this->recordWindow->setRecordSetName(record.recordSetName);

    int result = this->recordWindow->exec();

    if (result == QDialog::Accepted)
    {
        const QString recordId = this->recordWindow->getRecordId();
        const QString recordDisplayName = this->recordWindow->getRecordDisplayName();

        // Update record.
        this->updateRecord(recordId, recordDisplayName);

        // Get inherited fields.
        const RecordFieldValueMap inheritedFieldValues =
                recordsController.getInheritedFieldValues(recordId);

        // Update record fields.
        const QMap<QString, RecordFieldState::RecordFieldState> recordFields =
                this->recordWindow->getRecordFields();

        for (QMap<QString, RecordFieldState::RecordFieldState>::const_iterator it = recordFields.begin();
             it != recordFields.end();
             ++it)
        {
            const QString& fieldId = it.key();

            // Skip inherited fields.
            if (inheritedFieldValues.contains(fieldId))
            {
                continue;
            }

            // Check if field was added or removed.
            const bool fieldWasEnabled = record.fieldValues.contains(fieldId);
            const bool fieldIsEnabled = it.value() == RecordFieldState::Enabled;

            if (fieldIsEnabled && !fieldWasEnabled)
            {
                this->addRecordField(fieldId);
            }
            else if (fieldWasEnabled && !fieldIsEnabled)
            {
                this->removeRecordField(fieldId);
            }
        }

        // Move record, if necessary.
        const QString recordSetName = this->recordWindow->getRecordSetName();

        if (record.recordSetName != recordSetName)
        {
            recordsController.moveRecordToSet(record.id, recordSetName);
        }

        // Update view.
        this->refreshRecordTable();
        this->recordTreeWidget->updateRecordItem();
    }
}

void MainWindow::on_actionDuplicate_Record_triggered()
{
    if (!this->duplicateRecordWindow)
    {
        this->duplicateRecordWindow = new DuplicateRecordWindow(this);
    }

    const QString& recordId = this->recordTreeWidget->getSelectedRecordId();
    const QStringList recordIds = this->controller->getRecordsController().getRecordIds();

    if (recordId.isEmpty())
    {
        return;
    }

    // Disallow all existing record ids.
    this->duplicateRecordWindow->setRecordId(recordId);
    this->duplicateRecordWindow->setDisallowedRecordIds(recordIds);

    // Show window.
    int result = this->duplicateRecordWindow->exec();

    if (result != QDialog::Accepted)
    {
        return;
    }

    const QString& newRecordId = this->duplicateRecordWindow->getRecordId();

    // Update model.
    this->controller->getRecordsController().duplicateRecord(recordId, newRecordId);

    // Update view.
    this->recordTreeWidget->clear();
    this->refreshRecordTree();

    this->recordTreeWidget->selectRecord(newRecordId);
}

void MainWindow::on_actionRevert_Record_triggered()
{
    // Get record to revert.
    const QString& recordId = this->recordTreeWidget->getSelectedRecordId();

    if (recordId.isEmpty())
    {
        return;
    }

    // Check if read-only.
    const Record& record = this->controller->getRecordsController().getRecord(recordId);

    if (record.readOnly && !this->controller->getProjectIgnoreReadOnly())
    {
        this->showReadOnlyMessage(recordId);
        return;
    }

    // Show question.
    const QString& question = tr("Are you sure you want to revert %1 to its original state?").arg(recordId);

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
    this->controller->getRecordsController().revertRecord(recordId);

    // Update view.
    this->refreshRecordTable();
}

void MainWindow::on_actionRemove_Record_triggered()
{
    RecordTreeWidgetItem* recordItem = this->recordTreeWidget->getSelectedRecordItem();

    if (recordItem == 0)
    {
        return;
    }

    // Check if read-only.
    const QString recordId = recordItem->getId();
    const Record& record = this->controller->getRecordsController().getRecord(recordId);

    if (record.readOnly && !this->controller->getProjectIgnoreReadOnly())
    {
        this->showReadOnlyMessage(recordId);
        return;
    }

    // Update model.
    this->controller->getRecordsController().removeRecord(recordId);

    // Update view.
    if (recordItem->parent() != 0)
    {
        recordItem->parent()->removeChild(recordItem);
    }
    else
    {
        int index = this->recordTreeWidget->indexOfTopLevelItem(recordItem);
        this->recordTreeWidget->takeTopLevelItem(index);
    }

    delete recordItem;
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
    const QString& recordId = this->recordTreeWidget->getSelectedRecordId();

    if (recordId.isEmpty())
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
    QDesktopServices::openUrl(QUrl("https://github.com/npruehs/tome-editor/milestones"));
}

void MainWindow::on_actionOutput_triggered()
{
    this->showWindow(this->outputDockWidget);
}

void MainWindow::on_actionError_List_triggered()
{
    this->showWindow(this->errorListDockWidget);
}

void MainWindow::exportRecords(QAction* exportAction)
{
    // Get export template.
    QString exportTemplateName = exportAction->text();
    const RecordExportTemplate& exportTemplate =
            this->controller->getExportController().getRecordExportTemplate(exportTemplateName);

    // Build export file name suggestion.
    const QString suggestedFileName = this->controller->getProjectName() + exportTemplate.fileExtension;
    const QString suggestedFilePath = combinePaths(this->controller->getProjectPath(), suggestedFileName);
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
    QString recordId = this->recordTreeWidget->getSelectedRecordId();

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
    QString id = this->recordTreeWidget->getSelectedRecordId();
    const RecordFieldValueMap fieldValues =
            this->controller->getRecordsController().getRecordFieldValues(id);

    // Get current field data.
    const QString fieldId = fieldValues.keys()[index.row()];
    const QVariant fieldValue = fieldValues[fieldId];

    const FieldDefinition& field =
            this->controller->getFieldDefinitionsController().getFieldDefinition(fieldId);

    // Prepare window.
    if (!this->fieldValueWindow)
    {
        this->fieldValueWindow = new FieldValueWindow
                (this->controller->getFacetsController(),
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

        // Update model.
        this->controller->getRecordsController().updateRecordFieldValue(id, fieldId, fieldValue);

        // Update view.
        this->updateRecordRow(index.row());
    }
}

void MainWindow::treeWidgetDoubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index);
    this->on_actionEdit_Record_triggered();
}

void MainWindow::treeWidgetRecordReparented(const QString& recordId, const QString& newParentId)
{
    // Check if allowed.
    if (recordId == newParentId ||
            this->controller->getRecordsController().isAncestorOf(recordId, newParentId))
    {
        return;
    }

    // Check if read-only.
    const Record& record = this->controller->getRecordsController().getRecord(recordId);

    if (record.readOnly && !this->controller->getProjectIgnoreReadOnly())
    {
        this->showReadOnlyMessage(recordId);
        return;
    }

    // Update model.
    this->controller->getRecordsController().reparentRecord(recordId, newParentId);

    // Update view.
    this->recordTreeWidget->clear();
    this->refreshRecordTree();
    this->recordTreeWidget->selectRecord(recordId);
}

void MainWindow::treeWidgetSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected)
{
    Q_UNUSED(selected);
    Q_UNUSED(deselected);

    // Update field table.
    this->refreshRecordTable();
}

void MainWindow::addRecordField(const QString& fieldId)
{
    QString id = this->recordTreeWidget->getSelectedRecordId();
    const Record& record =
            this->controller->getRecordsController().getRecord(id);

    int index = findInsertionIndex(record.fieldValues.keys(), fieldId, qStringLessThanLowerCase);

    // Update model.
    this->controller->getRecordsController().addRecordField(record.id, fieldId);

    // Update view.
    this->recordFieldTableWidget->insertRow(index);
}

QString MainWindow::getReadOnlyMessage(const QString& recordId)
{
    return tr("The record %1 has been marked as read-only. You cannot edit, reparent or remove read-only records.").arg(recordId);
}

void MainWindow::openProject(QString path)
{
    if (path.isEmpty())
    {
        return;
    }

    try
    {
        this->controller->openProject(path);
    }
    catch (std::runtime_error& e)
    {
        this->updateRecentProjects();

        QMessageBox::critical(
                    this,
                    tr("Unable to open project"),
                    e.what(),
                    QMessageBox::Close,
                    QMessageBox::Close);
    }
}

void MainWindow::removeRecordField(const QString& fieldId)
{
    QString id = this->recordTreeWidget->getSelectedRecordId();
    const Record& record =
            this->controller->getRecordsController().getRecord(id);

    // Update model.
    this->controller->getRecordsController().removeRecordField(record.id, fieldId);

    // Update view.
    int index = findInsertionIndex(record.fieldValues.keys(), fieldId, qStringLessThanLowerCase);
    this->recordFieldTableWidget->removeRow(index);
}

void MainWindow::onFieldChanged()
{
    this->refreshRecordTable();
}

void MainWindow::onProgressChanged(const QString title, const QString text, const int currentValue, const int maximumValue)
{
    this->progressDialog->setWindowTitle(title);
    this->progressDialog->setLabelText(text);
    this->progressDialog->setMaximum(maximumValue);
    this->progressDialog->setValue(currentValue);

    if (currentValue >= maximumValue)
    {
        this->progressDialog->reset();
    }
}

void MainWindow::onProjectChanged(QSharedPointer<Project> project)
{
    Q_UNUSED(project);

    // Enable project-specific buttons.
    this->updateMenus();

    // Setup tree view.
    this->recordTreeWidget->clear();
    this->recordFieldTableWidget->setRowCount(0);

    this->refreshRecordTree();

    // Setup record exports.
    this->refreshExportMenu();

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

void MainWindow::onRecordFieldsChanged(const QString& recordId)
{
    if (this->recordTreeWidget->getSelectedRecordId() == recordId)
    {
        this->refreshRecordTable();
    }
}

void MainWindow::onRecordSetsChanged()
{
    this->refreshRecordTree();
}

void MainWindow::onRecordLinkActivated(const QString& recordId)
{
    this->recordTreeWidget->selectRecord(recordId);
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

void MainWindow::refreshRecordTree()
{
    this->recordTreeWidget->setRecords(this->controller->getRecordsController().getRecords());
}

void MainWindow::refreshRecordTable()
{
    const QString& id = this->recordTreeWidget->getSelectedRecordId();

    if (id.isEmpty() || !this->controller->getRecordsController().hasRecord(id))
    {
        // Clear table.
        this->recordFieldTableWidget->setRowCount(0);
        return;
    }

    // Get selected record.
    const RecordFieldValueMap fieldValues =
            this->controller->getRecordsController().getRecordFieldValues(id);

    // Update table.
    this->recordFieldTableWidget->setDescriptionColumnEnabled(
                this->controller->getSettingsController().getShowDescriptionColumnInsteadOfFieldTooltips());
    this->recordFieldTableWidget->setRowCount(fieldValues.size());

    for (int i = 0; i < this->recordFieldTableWidget->rowCount(); ++i)
    {
        this->updateRecordRow(i);
    }

    // Check if read-only.
    const Record& record = this->controller->getRecordsController().getRecord(id);

    if (record.readOnly && !this->controller->getProjectIgnoreReadOnly())
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

void MainWindow::showReadOnlyMessage(const QString& recordId)
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
}

void MainWindow::updateMenus()
{
    bool projectLoaded = this->controller->isProjectLoaded();

    this->ui->actionSave_Project->setEnabled(projectLoaded);

    this->ui->actionProject_Overview->setEnabled(projectLoaded);

    this->ui->actionField_Definions->setEnabled(projectLoaded);
    this->ui->actionManage_Components->setEnabled(projectLoaded);
    this->ui->actionManage_Custom_Types->setEnabled(projectLoaded);

    this->ui->actionNew_Record->setEnabled(projectLoaded);
    this->ui->actionEdit_Record->setEnabled(projectLoaded);
    this->ui->actionDuplicate_Record->setEnabled(projectLoaded);
    this->ui->actionRevert_Record->setEnabled(projectLoaded);
    this->ui->actionRemove_Record->setEnabled(projectLoaded);
    this->ui->actionFindRecord->setEnabled(projectLoaded);
    this->ui->actionFind_Usages->setEnabled(projectLoaded);

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

void MainWindow::updateRecord(const QString& id, const QString& displayName)
{
    QString selectedRecordId = this->recordTreeWidget->getSelectedRecordId();
    const Record& record =
            this->controller->getRecordsController().getRecord(selectedRecordId);

    bool needsSorting = record.displayName != displayName;

    // Update model.
    this->controller->getRecordsController().updateRecord(record.id, id, displayName);

    // Update view.
    RecordTreeWidgetItem* recordItem = this->recordTreeWidget->getSelectedRecordItem();
    recordItem->setId(id);
    recordItem->setDisplayName(displayName);

    // Sort by display name.
    if (needsSorting)
    {
        this->recordTreeWidget->sortItems(0, Qt::AscendingOrder);
    }
}

void MainWindow::updateRecordRow(const int i)
{
    // Get selected record.
    QString id = this->recordTreeWidget->getSelectedRecordId();

    // Update view.
    this->recordFieldTableWidget->setRecord(i, id);
}

void MainWindow::updateWindowTitle()
{
    // Get application version.
    QString windowTitle = "Tome " + QApplication::instance()->applicationVersion();

    if (this->controller->isProjectLoaded())
    {
        // Add project name.
        windowTitle += " - " + this->controller->getFullProjectPath();
    }

    this->setWindowTitle(windowTitle);
}
