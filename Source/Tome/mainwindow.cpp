#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <stdexcept>

#include <QDesktopServices>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardPaths>
#include <QTextStream>
#include <QXmlStreamWriter>

#include "Export/Controller/exportcontroller.h"
#include "Fields/Controller/fielddefinitionsetserializer.h"
#include "Projects/Model/project.h"
#include "Projects/Controller/projectserializer.h"
#include "Records/Controller/recordsetserializer.h"
#include "Settings/Controller/settingscontroller.h"
#include "Types/builtintype.h"
#include "Util/pathutils.h"
#include "Util/vectorutils.h"


using namespace Tome;

const QString MainWindow::FieldDefinitionFileExtension = ".tfields";
const QString MainWindow::ProjectFileExtension = ".tproj";
const QString MainWindow::RecordFileExtension = ".tdata";
const QString MainWindow::RecordExportComponentTemplateExtension = ".texportc";
const QString MainWindow::RecordExportComponentDelimiterExtension = ".texportcd";
const QString MainWindow::RecordExportRecordFileTemplateExtension = ".texportf";
const QString MainWindow::RecordExportRecordTemplateExtension = ".texportr";
const QString MainWindow::RecordExportRecordDelimiterExtension = ".texportrd";
const QString MainWindow::RecordExportFieldValueTemplateExtension = ".texportv";
const QString MainWindow::RecordExportFieldValueDelimiterExtension = ".texportvd";


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    controller(QSharedPointer<Controller>::create()),
    aboutWindow(0),
    componentsWindow(0),
    customTypesWindow(0),
    fieldDefinitionsWindow(0),
    fieldValueWindow(0),
    newProjectWindow(0),
    recordWindow(0)
{
    ui->setupUi(this);

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

    // Maximize window.
    this->showMaximized();

    // Set window title.
    this->updateWindowTitle();

    // Can't access some functionality until project created or loaded.
    this->updateMenus();
    this->updateRecentProjects();

    // Setup view.
    this->ui->tableWidget->setColumnCount(2);

    QStringList headers;
    headers << tr("Field");
    headers << tr("Value");
    this->ui->tableWidget->setHorizontalHeaderLabels(headers);
}

MainWindow::~MainWindow()
{
    delete ui;
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
                    this);
    }

    this->showWindow(this->fieldDefinitionsWindow);
}


void MainWindow::on_actionManage_Components_triggered()
{
    if (!this->componentsWindow)
    {
        this->componentsWindow = new ComponentsWindow(this->controller->getComponentsController(), this);
    }

    this->showWindow(this->componentsWindow);
}

void MainWindow::on_actionManage_Custom_Types_triggered()
{
    if (!this->customTypesWindow)
    {
        this->customTypesWindow = new CustomTypesWindow(this->controller->getTypesController(), this);
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

        this->createNewProject(projectName, projectPath);
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
    this->saveProject(this->project);
}

void MainWindow::on_actionNew_Record_triggered()
{
    if (!this->recordWindow)
    {
        this->recordWindow = new RecordWindow(this);
    }

    // Add fields.
    this->recordWindow->clearRecordFields();

    for (int i = 0; i < this->project->fieldDefinitionSets.size(); ++i)
    {
        const FieldDefinitionSet& fieldDefinitionSet = this->project->fieldDefinitionSets[i];

        for (int j = 0; j < fieldDefinitionSet.fieldDefinitions.size(); ++j)
        {
            const FieldDefinition& fieldDefinition = fieldDefinitionSet.fieldDefinitions[j];
            this->recordWindow->setRecordField(fieldDefinition.id, fieldDefinition.component, false);
        }
    }

    int result = this->recordWindow->exec();

    if (result == QDialog::Accepted)
    {
        const QString& recordId = this->recordWindow->getRecordId();
        const QString& recordDisplayName = this->recordWindow->getRecordDisplayName();

        // Update model.
        QSharedPointer<Record> record = QSharedPointer<Record>::create();
        record->id = recordId;
        record->displayName = recordDisplayName;

        RecordList& records = this->project->recordSets[0].records;
        int index = findInsertionIndex(records, *record.data(), recordLessThanDisplayName);
        records.insert(index, *record.data());

        // Insert tree view item.
        QTreeWidgetItem* newItem = new QTreeWidgetItem((QTreeWidget*)0, QStringList(recordDisplayName));
        this->ui->treeWidget->insertTopLevelItem(index, newItem);

        // Select new record.
        this->ui->treeWidget->setCurrentItem(newItem);

        // Add record fields.
        const QMap<QString, bool> recordFields = this->recordWindow->getRecordFields();

        for (QMap<QString, bool>::const_iterator it = recordFields.begin();
             it != recordFields.end();
             ++it)
        {
            const QString& fieldId = it.key();
            const bool fieldEnabled = it.value();

            if (fieldEnabled)
            {
                this->addRecordField(fieldId);
            }
        }
    }
}

void MainWindow::on_actionEdit_Record_triggered()
{
    const QString& displayName = getSelectedRecordDisplayName();

    if (displayName.isEmpty())
    {
        return;
    }

    // Get selected record.
    const Record& record =
            this->controller->getRecordsController().getRecordByDisplayName(displayName);

    // Show window.
    if (!this->recordWindow)
    {
        this->recordWindow = new RecordWindow(this);
    }

    // Update view.
    this->recordWindow->setRecordId(record.id);
    this->recordWindow->setRecordDisplayName(record.displayName);

    this->recordWindow->clearRecordFields();

    for (int i = 0; i < this->project->fieldDefinitionSets.size(); ++i)
    {
        const FieldDefinitionSet& fieldDefinitionSet = this->project->fieldDefinitionSets[i];

        for (int j = 0; j < fieldDefinitionSet.fieldDefinitions.size(); ++j)
        {
            const FieldDefinition& fieldDefinition = fieldDefinitionSet.fieldDefinitions[j];

            // Check if record contains field.
            bool fieldEnabled = record.fieldValues.contains(fieldDefinition.id);

            // Add to view.
            this->recordWindow->setRecordField(fieldDefinition.id, fieldDefinition.component, fieldEnabled);
        }
    }

    int result = this->recordWindow->exec();

    if (result == QDialog::Accepted)
    {
        const QString recordId = this->recordWindow->getRecordId();
        const QString recordDisplayName = this->recordWindow->getRecordDisplayName();

        // Update record.
        this->updateRecord(recordId, recordDisplayName);

        // Update record fields.
        const QMap<QString, bool> recordFields = this->recordWindow->getRecordFields();

        for (QMap<QString, bool>::const_iterator it = recordFields.begin();
             it != recordFields.end();
             ++it)
        {
            const QString& fieldId = it.key();

            const bool fieldWasEnabled = record.fieldValues.contains(fieldId);
            const bool fieldIsEnabled = it.value();

            if (fieldIsEnabled && !fieldWasEnabled)
            {
                this->addRecordField(fieldId);
            }
            else if (fieldWasEnabled && !fieldIsEnabled)
            {
                this->removeRecordField(fieldId);
            }
        }
    }
}

void MainWindow::on_actionRemove_Record_triggered()
{
    QModelIndexList selectedIndexes = this->ui->treeWidget->selectionModel()->selectedIndexes();

    if (selectedIndexes.empty())
    {
        return;
    }

    QModelIndex currentIndex = selectedIndexes.first();

    if (!currentIndex.isValid())
    {
        return;
    }

    // Update model.
    this->project->recordSets[0].records.removeAt(currentIndex.row());

    // Update view.
    this->ui->treeWidget->takeTopLevelItem(currentIndex.row());
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

void MainWindow::on_treeWidget_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index);
    this->on_actionEdit_Record_triggered();
}

void MainWindow::on_tableWidget_doubleClicked(const QModelIndex &index)
{
    QString recordDisplayName = this->getSelectedRecordDisplayName();
    const Record& record =
            this->controller->getRecordsController().getRecordByDisplayName(recordDisplayName);

    // Get current field data.
    const QString fieldId = record.fieldValues.keys()[index.row()];
    const QString fieldValue = record.fieldValues[fieldId];

    const FieldDefinition& field =
            this->controller->getFieldDefinitionsController().getFieldDefinition(fieldId);

    // Prepare window.
    if (!this->fieldValueWindow)
    {
        this->fieldValueWindow = new FieldValueWindow(this);
    }

    // Update view.
    this->fieldValueWindow->setFieldDisplayName(field.displayName);
    this->fieldValueWindow->setFieldDescription(field.description);

    if (field.fieldType == BuiltInType::Reference)
    {
        QStringList recordNames = this->controller->getRecordsController().getRecordNames();

        // Allow clearing the field.
        recordNames << QString();

        this->fieldValueWindow->setFieldType(field.fieldType);
        this->fieldValueWindow->setEnumeration(recordNames);
    }
    else
    {
        const bool isCustomType = this->controller->getTypesController().isCustomType(field.fieldType);

        if (isCustomType)
        {
            const CustomType& type =
                    this->controller->getTypesController().getCustomType(field.fieldType);

            this->fieldValueWindow->setCustomFieldType(type);
        }
        else
        {
            // Default built-in type.
            this->fieldValueWindow->setFieldType(field.fieldType);
        }
    }

    this->fieldValueWindow->setFieldValue(fieldValue);

    // Show window.
    int result = this->fieldValueWindow->exec();

    if (result == QDialog::Accepted)
    {
        QString fieldValue = this->fieldValueWindow->getFieldValue();

        // Update model.
        this->controller->getRecordsController().updateRecordFieldValue(record.id, fieldId, fieldValue);

        // Update view.
        this->updateRecordRow(index.row());
    }
}

void MainWindow::exportRecords(QAction* exportAction)
{
    // Get export template.
    QString exportTemplateName = exportAction->text();
    QSharedPointer<RecordExportTemplate> exportTemplate =
            this->project->recordExportTemplates[exportTemplateName];

    // Build export file name suggestion.
    const QString suggestedFileName = this->project->name + exportTemplate->fileExtension;
    const QString suggestedFilePath = combinePaths(this->project->path, suggestedFileName);

    // Show file dialog.
    QString filePath = QFileDialog::getSaveFileName(this,
                                                    tr("Export Records"),
                                                    suggestedFilePath);

    if (filePath.isEmpty())
    {
        return;
    }

    // Export records.
    QFile file(filePath);

    if (file.open(QIODevice::ReadWrite | QIODevice::Truncate))
    {
        this->controller->getExportController().exportRecords(*exportTemplate.data(), file);
    }
    else
    {
        QMessageBox::critical(
                    this,
                    tr("Unable to export records"),
                    tr("Destination file could not be written:\r\n") + filePath,
                    QMessageBox::Close,
                    QMessageBox::Close);
        return;
    }
}

void MainWindow::openRecentProject(QAction* recentProjectAction)
{
    QString path = recentProjectAction->text();
    this->openProject(path);
}

void MainWindow::treeViewSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected)
{
    Q_UNUSED(deselected);

    if (selected.empty())
    {
        this->ui->tableWidget->clear();
        return;
    }

    QModelIndex selectedIndex = selected.first().indexes().first();

    if (!selectedIndex.isValid())
    {
        return;
    }

    const QString& displayName = selectedIndex.data(Qt::DisplayRole).toString();

    if (displayName.isEmpty())
    {
        return;
    }

    // Get selected record.
    const Record& record =
            this->controller->getRecordsController().getRecordByDisplayName(displayName);

    // Update field table.
    this->ui->tableWidget->setRowCount(record.fieldValues.size());

    for (int i = 0; i < record.fieldValues.size(); ++i)
    {
        this->updateRecordRow(i);
    }
}

void MainWindow::addRecordField(const QString& fieldId)
{
    QString recordDisplayName = this->getSelectedRecordDisplayName();
    const Record& record =
            this->controller->getRecordsController().getRecordByDisplayName(recordDisplayName);

    int index = findInsertionIndex(record.fieldValues.keys(), fieldId);

    // Update model.
    this->controller->getRecordsController().addRecordField(record.id, fieldId);

    // Update view.
    this->ui->tableWidget->insertRow(index);
    this->updateRecordRow(index);
}

void MainWindow::createNewProject(const QString &projectName, const QString &projectPath)
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
    if (this->saveProject(newProject))
    {
        // Set project reference.
        this->setProject(newProject);
    }
}

QString MainWindow::getFullProjectPath() const
{
    return this->getFullProjectPath(this->project);
}

QString MainWindow::getFullProjectPath(QSharedPointer<Project> project) const
{
    if (project == 0)
    {
        return QString();
    }

    return combinePaths(project->path, project->name + ProjectFileExtension);
}

QString MainWindow::getSelectedRecordDisplayName() const
{
    QModelIndexList selectedIndexes = this->ui->treeWidget->selectionModel()->selectedIndexes();

    if (selectedIndexes.empty())
    {
        return QString();
    }

    QModelIndex currentIndex = selectedIndexes.first();

    if (!currentIndex.isValid())
    {
        return QString();
    }

    return currentIndex.data(Qt::DisplayRole).toString();
}

void MainWindow::openProject(QString projectFileName)
{
    if (projectFileName.count() <= 0)
    {
        return;
    }

    // Open project file.
    QSharedPointer<QFile> projectFile = QSharedPointer<QFile>::create(projectFileName);
    QSharedPointer<QFileInfo> projectFileInfo = QSharedPointer<QFileInfo>::create(projectFileName);
    const QString projectPath = projectFileInfo->path();

    if (projectFile->open(QIODevice::ReadOnly))
    {
        // Load project from file.
        QSharedPointer<ProjectSerializer> projectSerializer =
                QSharedPointer<ProjectSerializer>::create();
        QSharedPointer<Project> project = QSharedPointer<Project>::create();
        project->path = projectPath;

        try
        {
            projectSerializer->deserialize(projectFile, project);
        }
        catch (const std::runtime_error& e)
        {
            QMessageBox::critical(
                        this,
                        tr("Unable to open project"),
                        tr("File could not be read: ") + projectFileName + "\r\n" + e.what(),
                        QMessageBox::Close,
                        QMessageBox::Close);
            return;
        }

        // Load field definition files.
        QSharedPointer<FieldDefinitionSetSerializer> fieldDefinitionSerializer =
                QSharedPointer<FieldDefinitionSetSerializer>::create();

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
                    fieldDefinitionSerializer->deserialize(fieldDefinitionFile, fieldDefinitionSet);
                }
                catch (const std::runtime_error& e)
                {
                    QMessageBox::critical(
                                this,
                                tr("Unable to open project"),
                                tr("File could not be read: ") + fullFieldDefinitionSetPath + "\r\n" + e.what(),
                                QMessageBox::Close,
                                QMessageBox::Close);
                    return;
                }
            }
            else
            {
                QMessageBox::critical(
                            this,
                            tr("Unable to open project"),
                            tr("File could not be read:\r\n") + fullFieldDefinitionSetPath,
                            QMessageBox::Close,
                            QMessageBox::Close);
                return;
            }
        }

        // Load record files.
        QSharedPointer<RecordSetSerializer> recordSetSerializer =
                QSharedPointer<RecordSetSerializer>::create();

        for (int i = 0; i < project->recordSets.size(); ++i)
        {
            RecordSet& recordSet = project->recordSets[i];

            // Open record file.
            const QString fullRecordSetPath = combinePaths(projectPath, recordSet.name + RecordFileExtension);
            QSharedPointer<QFile> recordFile = QSharedPointer<QFile>::create(fullRecordSetPath);

            if (recordFile->open(QIODevice::ReadOnly))
            {
                try
                {
                    recordSetSerializer->deserialize(recordFile, recordSet);
                }
                catch (const std::runtime_error& e)
                {
                    QMessageBox::critical(
                                this,
                                tr("Unable to open project"),
                                tr("File could not be read: ") + fullRecordSetPath + "\r\n" + e.what(),
                                QMessageBox::Close,
                                QMessageBox::Close);
                    return;
                }
            }
            else
            {
                QMessageBox::critical(
                            this,
                            tr("Unable to open project"),
                            tr("File could not be read:\r\n") + fullRecordSetPath,
                            QMessageBox::Close,
                            QMessageBox::Close);
                return;
            }
        }

        // Load record export template files.
        for (QMap<QString, QSharedPointer<RecordExportTemplate> >::iterator it = project->recordExportTemplates.begin();
             it != project->recordExportTemplates.end();
             ++it)
        {
            QSharedPointer<RecordExportTemplate> exportTemplate = it.value();

            try
            {
                exportTemplate->fieldValueDelimiter =
                        this->readProjectFile(projectPath, exportTemplate->name + RecordExportFieldValueDelimiterExtension);
                exportTemplate->fieldValueTemplate =
                        this->readProjectFile(projectPath, exportTemplate->name + RecordExportFieldValueTemplateExtension);
                exportTemplate->recordDelimiter =
                        this->readProjectFile(projectPath, exportTemplate->name + RecordExportRecordDelimiterExtension);
                exportTemplate->recordFileTemplate =
                        this->readProjectFile(projectPath, exportTemplate->name + RecordExportRecordFileTemplateExtension);
                exportTemplate->recordTemplate =
                        this->readProjectFile(projectPath, exportTemplate->name + RecordExportRecordTemplateExtension);
                exportTemplate->componentDelimiter =
                        this->readProjectFile(projectPath, exportTemplate->name + RecordExportComponentDelimiterExtension);
                exportTemplate->componentTemplate =
                        this->readProjectFile(projectPath, exportTemplate->name + RecordExportComponentTemplateExtension);
            }
            catch (const std::runtime_error& e)
            {
                QMessageBox::critical(
                            this,
                            tr("Unable to open project"),
                            tr("File could not be read:\r\n") + e.what(),
                            QMessageBox::Close,
                            QMessageBox::Close);
                return;
            }
        }

        // Add to recent projects.
        this->controller->getSettingsController().addRecentProject(projectFileName);
        this->updateRecentProjects();

        // Set project reference.
        this->setProject(project);
    }
    else
    {
        // Remove from recent projects.
        this->controller->getSettingsController().removeRecentProject(projectFileName);
        this->updateRecentProjects();

        QMessageBox::critical(
                    this,
                    tr("Unable to open project"),
                    tr("File could not be read:\r\n") + projectFileName,
                    QMessageBox::Close,
                    QMessageBox::Close);
        return;
    }
}

QString MainWindow::readProjectFile(QString projectPath, QString fileName)
{
    // Open export template file.
    const QString fullPath = combinePaths(projectPath, fileName);
    QSharedPointer<QFile> file = QSharedPointer<QFile>::create(fullPath);

    if (file->open(QIODevice::ReadOnly))
    {
        QTextStream textStream(file.data());
        return textStream.readAll();
    }
    else
    {
        throw std::runtime_error(fullPath.toStdString());
    }
}

void MainWindow::removeRecordField(const QString& fieldId)
{
    QString recordDisplayName = this->getSelectedRecordDisplayName();
    const Record& record =
            this->controller->getRecordsController().getRecordByDisplayName(recordDisplayName);

    // Update model.
    this->controller->getRecordsController().removeRecordField(record.id, fieldId);

    // Update view.
    int index = findInsertionIndex(record.fieldValues.keys(), fieldId);
    this->ui->tableWidget->removeRow(index);
}

bool MainWindow::saveProject(QSharedPointer<Project> project)
{
    QString& projectPath = project->path;

    QSharedPointer<ProjectSerializer> projectSerializer =
            QSharedPointer<ProjectSerializer>::create();

    // Build file name.
    const QString fullProjectPath = this->getFullProjectPath(project);

    // Write project file.
    QSharedPointer<QFile> projectFile = QSharedPointer<QFile>::create(fullProjectPath);

    if (projectFile->open(QIODevice::ReadWrite | QIODevice::Truncate))
    {
        projectSerializer->serialize(projectFile, project);
    }
    else
    {
        QMessageBox::critical(
                    this,
                    tr("Unable to create project"),
                    tr("Destination file could not be written:\r\n") + fullProjectPath,
                    QMessageBox::Close,
                    QMessageBox::Close);
        return false;
    }

    // Write field definition sets.
    QSharedPointer<FieldDefinitionSetSerializer> fieldDefinitionSetSerializer =
            QSharedPointer<FieldDefinitionSetSerializer>::create();

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
            fieldDefinitionSetSerializer->serialize(fieldDefinitionSetFile, fieldDefinitionSet);
        }
        else
        {
            QMessageBox::critical(
                        this,
                        tr("Unable to create project"),
                        tr("Destination file could not be written:\r\n") + fullFieldDefinitionSetPath,
                        QMessageBox::Close,
                        QMessageBox::Close);
            return false;
        }
    }


    // Write record sets.
    QSharedPointer<Tome::RecordSetSerializer> recordSetSerializer =
            QSharedPointer<Tome::RecordSetSerializer>::create();

    for (int i = 0; i < project->recordSets.size(); ++i)
    {
        const RecordSet& recordSet = project->recordSets[i];

        // Build file name.
        const QString recordSetFileName = recordSet.name + RecordFileExtension;
        const QString fullRecordSetPath = Tome::combinePaths(projectPath, recordSetFileName);

        // Write file.
        QSharedPointer<QFile> recordSetFile = QSharedPointer<QFile>::create(fullRecordSetPath);

        if (recordSetFile->open(QIODevice::ReadWrite | QIODevice::Truncate))
        {
            recordSetSerializer->serialize(recordSetFile, recordSet);
        }
        else
        {
            QMessageBox::critical(
                        this,
                        tr("Unable to create project"),
                        tr("Destination file could not be written:\r\n") + fullRecordSetPath,
                        QMessageBox::Close,
                        QMessageBox::Close);
            return false;
        }
    }

    return true;
}

void MainWindow::setProject(QSharedPointer<Project> project)
{
    this->project = project;

    // Update controllers.
    this->controller->setProject(project);

    // Enable project-specific buttons.
    this->updateMenus();

    // Setup tree view.
    this->ui->treeWidget->setColumnCount(1);

    const RecordList& records = this->project->recordSets[0].records;
    QList<QTreeWidgetItem *> items;

    for (int i = 0; i < records.size(); ++i)
    {
        const Record& record = records[i];
        items.append(new QTreeWidgetItem((QTreeWidget*)0, QStringList(record.displayName)));
    }

    this->ui->treeWidget->insertTopLevelItems(0, items);
    this->ui->treeWidget->expandAll();

    // Listen for selection changes.
    connect(
                this->ui->treeWidget->selectionModel(),
                SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
                SLOT(treeViewSelectionChanged(const QItemSelection &, const QItemSelection &))
                );

    // Setup record exports.
    this->ui->menuExport->clear();

    for (QMap<QString, QSharedPointer<RecordExportTemplate> >::iterator it = this->project->recordExportTemplates.begin();
         it != this->project->recordExportTemplates.end();
         ++it)
    {
        QAction* exportAction = new QAction(it.key(), this);
        this->ui->menuExport->addAction(exportAction);
    }

    // Update title.
    this->updateWindowTitle();
}

void MainWindow::showWindow(QWidget* widget)
{
    widget->show();
    widget->raise();
    widget->activateWindow();
}

void MainWindow::updateMenus()
{
    bool projectLoaded = this->project != 0;

    this->ui->actionSave_Project->setEnabled(projectLoaded);

    this->ui->actionField_Definions->setEnabled(projectLoaded);
    this->ui->actionManage_Components->setEnabled(projectLoaded);
    this->ui->actionManage_Custom_Types->setEnabled(projectLoaded);

    this->ui->actionNew_Record->setEnabled(projectLoaded);
    this->ui->actionEdit_Record->setEnabled(projectLoaded);
    this->ui->actionRemove_Record->setEnabled(projectLoaded);
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
    QString recordDisplayName = this->getSelectedRecordDisplayName();
    const Record& record =
            this->controller->getRecordsController().getRecordByDisplayName(recordDisplayName);

    bool needsSorting = record.displayName != displayName;

    // Update model.
    this->controller->getRecordsController().updateRecord(record.id, id, displayName);

    // Update view.
    QModelIndexList selectedIndexes = this->ui->treeWidget->selectionModel()->selectedIndexes();

    if (selectedIndexes.empty())
    {
        return;
    }

    QModelIndex currentIndex = selectedIndexes.first();

    if (!currentIndex.isValid())
    {
        return;
    }

    QTreeWidgetItem* item = this->ui->treeWidget->topLevelItem(currentIndex.row());
    item->setText(0, displayName);

    // Sort by display name.
    if (needsSorting)
    {
        this->ui->treeWidget->sortItems(0, Qt::AscendingOrder);
    }
}

void MainWindow::updateRecordRow(int i)
{
    // Show field and value.
    QString selectedRecordDisplayName = this->getSelectedRecordDisplayName();
    const Record& record =
            this->controller->getRecordsController().getRecordByDisplayName(selectedRecordDisplayName);
    QString key = record.fieldValues.keys()[i];
    QString value = record.fieldValues[key];

    this->ui->tableWidget->setItem(i, 0, new QTableWidgetItem(key));
    this->ui->tableWidget->setItem(i, 1, new QTableWidgetItem(value));

    // Show field description as tooltip.
    const FieldDefinition& field =
            this->controller->getFieldDefinitionsController().getFieldDefinition(key);
    this->ui->tableWidget->item(i, 0)->setData(Qt::ToolTipRole, field.description);
    this->ui->tableWidget->item(i, 1)->setData(Qt::ToolTipRole, field.description);

    // Show color preview.
    if (field.fieldType == BuiltInType::Color)
    {
        QColor color;
        color.setNamedColor(value);
        this->ui->tableWidget->item(i, 1)->setData(Qt::DecorationRole, color);
    }
}

void MainWindow::updateWindowTitle()
{
    // Get application version.
    QString windowTitle = "Tome " + QApplication::instance()->applicationVersion();

    if (this->project != 0)
    {
        // Add project name.
        windowTitle += " - " + this->getFullProjectPath();
    }

    this->setWindowTitle(windowTitle);
}
