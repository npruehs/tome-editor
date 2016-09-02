#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <stdexcept>

#include <QDesktopServices>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QLabel>
#include <QMessageBox>
#include <QStandardPaths>
#include <QTextStream>
#include <QXmlStreamWriter>

#include "controller.h"
#include "../Features/Components/View/componentswindow.h"
#include "../Features/Components/Controller/componentscontroller.h"
#include "../Features/Export/Controller/exportcontroller.h"
#include "../Features/Fields/Controller/fielddefinitionsetserializer.h"
#include "../Features/Fields/Controller/fielddefinitionscontroller.h"
#include "../Features/Fields/View/fielddefinitionswindow.h"
#include "../Features/Fields/View/fieldvaluewindow.h"
#include "../Features/Help/View/aboutwindow.h"
#include "../Features/Projects/Model/project.h"
#include "../Features/Projects/Controller/projectserializer.h"
#include "../Features/Projects/View/newprojectwindow.h"
#include "../Features/Records/Controller/recordscontroller.h"
#include "../Features/Records/Controller/recordsetserializer.h"
#include "../Features/Records/Model/recordfieldstate.h"
#include "../Features/Records/View/recordfieldstablewidget.h"
#include "../Features/Records/View/recordtreewidget.h"
#include "../Features/Records/View/recordtreewidgetitem.h"
#include "../Features/Records/View/recordwindow.h"
#include "../Features/Records/View/duplicaterecordwindow.h"
#include "../Features/Search/Controller/findusagescontroller.h"
#include "../Features/Search/View/searchresultsdockwidget.h"
#include "../Features/Settings/Controller/settingscontroller.h"
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


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    controller(new Controller()),
    aboutWindow(0),
    componentsWindow(0),
    customTypesWindow(0),
    fieldDefinitionsWindow(0),
    fieldValueWindow(0),
    newProjectWindow(0),
    recordWindow(0),
    duplicateRecordWindow(0)
{
    ui->setupUi(this);

    // Add record tree.
    this->recordTreeWidget = new RecordTreeWidget(this->controller->getRecordsController());
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

    // Hide all dock widgets until required.
    this->searchResultsDockWidget->close();
    this->errorListDockWidget->close();

    // Connect signals.
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
                &this->controller->getFindUsagesController(),
                SIGNAL(searchResultChanged(const QString&, const Tome::SearchResultList)),
                SLOT(searchResultChanged(const QString&, const Tome::SearchResultList))
                );

    // Maximize window.
    this->showMaximized();

    // Set window title.
    this->updateWindowTitle();

    // Can't access some functionality until project created or loaded.
    this->updateMenus();
    this->updateRecentProjects();
}

MainWindow::~MainWindow()
{
    delete this->ui;

    delete this->controller;

    delete this->recordTreeWidget;
    delete this->recordFieldTableWidget;
    delete this->errorListDockWidget;

    delete this->aboutWindow;
    delete this->componentsWindow;
    delete this->customTypesWindow;
    delete this->fieldDefinitionsWindow;
    delete this->fieldValueWindow;
    delete this->newProjectWindow;
    delete this->recordWindow;
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
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
                    this->controller->getFieldDefinitionsController(),
                    this->controller->getFindUsagesController(),
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
            this->onProjectChanged();
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

    // Add fields.
    const FieldDefinitionList& fieldDefinitions =
            this->controller->getFieldDefinitionsController().getFieldDefinitions();
    const QStringList recordIds = this->controller->getRecordsController().getRecordIds();
    const ComponentList& componentDefinitions =
            this->controller->getComponentsController().getComponents();

    // Disallow all existing record ids.
    this->recordWindow->setDisallowedRecordIds(recordIds);

    // Set fields.
    this->recordWindow->setRecordFields(fieldDefinitions);
    // Set components.
    this->recordWindow->setRecordComponents(componentDefinitions);

    // Set components.
    this->recordWindow->setRecordComponents(componentDefinitions);

    // Show window.
    int result = this->recordWindow->exec();

    if (result == QDialog::Accepted)
    {
        const QString& recordId = this->recordWindow->getRecordId();
        const QString& recordDisplayName = this->recordWindow->getRecordDisplayName();

        // Update model.
        this->controller->getRecordsController().addRecord(recordId, recordDisplayName);

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
        this->recordTreeWidget->updateRecordIcon();
    }
}

void MainWindow::on_actionEdit_Record_triggered()
{
    const QString& id = this->recordTreeWidget->getSelectedRecordId();

    if (id.isEmpty())
    {
        return;
    }

    // Get selected record.
    const Record& record = this->controller->getRecordsController().getRecord(id);

    // Show window.
    if (!this->recordWindow)
    {
        this->recordWindow = new RecordWindow(this);
    }

    // Set record id and name.
    this->recordWindow->setRecordId(record.id);
    this->recordWindow->setRecordDisplayName(record.displayName);

    // Disallow all other record ids.
    QStringList recordIds = this->controller->getRecordsController().getRecordIds();
    recordIds.removeOne(record.id);

    this->recordWindow->setDisallowedRecordIds(recordIds);

    // Set fields.
    const FieldDefinitionList& fieldDefinitions =
            this->controller->getFieldDefinitionsController().getFieldDefinitions();
    const RecordFieldValueMap inheritedFieldValues =
            this->controller->getRecordsController().getInheritedFieldValues(record.id);
    const ComponentList& componentDefinitions =
            this->controller->getComponentsController().getComponents();

    this->recordWindow->setRecordFields(fieldDefinitions, componentDefinitions, record.fieldValues, inheritedFieldValues);

    int result = this->recordWindow->exec();

    if (result == QDialog::Accepted)
    {
        const QString recordId = this->recordWindow->getRecordId();
        const QString recordDisplayName = this->recordWindow->getRecordDisplayName();

        // Update record.
        this->updateRecord(recordId, recordDisplayName);

        // Get inherited fields.
        const RecordFieldValueMap inheritedFieldValues =
                this->controller->getRecordsController().getInheritedFieldValues(recordId);

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

        // Update view.
        this->refreshRecordTable();
        this->recordTreeWidget->updateRecordIcon();
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

    // Show question.
    const QString& question = QString(tr("Are you sure you want to revert %1 to its original state?")).arg(recordId);

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

    // Update model.
    this->controller->getRecordsController().removeRecord(recordItem->getId());

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

void MainWindow::on_actionFind_Usages_triggered()
{
    // Find usages.
    const QString& recordId = this->recordTreeWidget->getSelectedRecordId();
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
    QDesktopServices::openUrl(QUrl("https://github.com/npruehs/tome-editor/issues/new"));
}

void MainWindow::on_actionReleases_triggered()
{
    QDesktopServices::openUrl(QUrl("https://github.com/npruehs/tome-editor/releases"));
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
                (this->controller->getRecordsController(),
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

    // Update model.
    this->controller->getRecordsController().reparentRecord(recordId, newParentId);

    // Update view.
    this->recordTreeWidget->clear();
    this->refreshRecordTree();
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

void MainWindow::openProject(QString path)
{
    if (path.isEmpty())
    {
        return;
    }

    try
    {
        this->controller->openProject(path);
        this->onProjectChanged();
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

void MainWindow::onProjectChanged()
{
    // Enable project-specific buttons.
    this->updateMenus();

    // Setup tree view.
    this->recordTreeWidget->clear();
    this->recordFieldTableWidget->setRowCount(0);

    this->refreshRecordTree();

    // Setup record exports.
    this->ui->menuExport->clear();

    const RecordExportTemplateMap& recordExportTemplateMap =
            this->controller->getExportController().getRecordExportTemplates();

    for (RecordExportTemplateMap::const_iterator it = recordExportTemplateMap.begin();
         it != recordExportTemplateMap.end();
         ++it)
    {
        QAction* exportAction = new QAction(it.key(), this);
        this->ui->menuExport->addAction(exportAction);
    }

    // Update title.
    this->updateWindowTitle();

    // Update recent projects.
    this->updateRecentProjects();
}

void MainWindow::refreshErrorList()
{
    this->errorListDockWidget->showMessages(this->messages);
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
    this->recordFieldTableWidget->setRowCount(fieldValues.size());

    for (int i = 0; i < this->recordFieldTableWidget->rowCount(); ++i)
    {
        this->updateRecordRow(i);
    }

    this->recordTreeWidget->updateRecordIcon( this->recordTreeWidget->getSelectedRecordItem() );
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

    this->ui->actionField_Definions->setEnabled(projectLoaded);
    this->ui->actionManage_Components->setEnabled(projectLoaded);
    this->ui->actionManage_Custom_Types->setEnabled(projectLoaded);

    this->ui->actionNew_Record->setEnabled(projectLoaded);
    this->ui->actionEdit_Record->setEnabled(projectLoaded);
    this->ui->actionRemove_Record->setEnabled(projectLoaded);

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

void MainWindow::updateRecordRow(int i)
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
