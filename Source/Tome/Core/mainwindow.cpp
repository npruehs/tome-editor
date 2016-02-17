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

#include "../Features/Export/Controller/exportcontroller.h"
#include "../Features/Fields/Controller/fielddefinitionsetserializer.h"
#include "../Features/Projects/Model/project.h"
#include "../Features/Projects/Controller/projectserializer.h"
#include "../Features/Records/Controller/recordsetserializer.h"
#include "../Features/Settings/Controller/settingscontroller.h"
#include "../Features/Types/Model/builtintype.h"
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
    this->resetFields();
}

MainWindow::~MainWindow()
{
    delete this->ui;
    delete this->controller;

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
    this->recordWindow->clearRecordFields();

    const FieldDefinitionSetList& fieldDefinitionSetList =
            this->controller->getFieldDefinitionsController().getFieldDefinitionSets();

    for (int i = 0; i < fieldDefinitionSetList.size(); ++i)
    {
        const FieldDefinitionSet& fieldDefinitionSet = fieldDefinitionSetList[i];

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
        const Record& record = this->controller->getRecordsController().addRecord(recordId, recordDisplayName);

        // Update view.
        int index = this->controller->getRecordsController().indexOf(record);
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

    const FieldDefinitionSetList& fieldDefinitionSets =
            this->controller->getFieldDefinitionsController().getFieldDefinitionSets();

    for (int i = 0; i < fieldDefinitionSets.size(); ++i)
    {
        const FieldDefinitionSet& fieldDefinitionSet = fieldDefinitionSets[i];

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

    int index = currentIndex.row();

    // Update model.
    this->controller->getRecordsController().removeRecordAt(index);

    // Update view.
    this->ui->treeWidget->takeTopLevelItem(index);
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
    const QVariant fieldValue = record.fieldValues[fieldId];

    const FieldDefinition& field =
            this->controller->getFieldDefinitionsController().getFieldDefinition(fieldId);

    // Prepare window.
    if (!this->fieldValueWindow)
    {
        this->fieldValueWindow = new FieldValueWindow
                (this->controller->getRecordsController(),
                 this->controller->getTypesController(),
                 this);
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
        this->controller->getRecordsController().updateRecordFieldValue(record.id, fieldId, fieldValue);

        // Update view.
        this->updateRecordRow(index.row());
    }
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

    // Show file dialog.
    QString filePath = QFileDialog::getSaveFileName(this,
                                                    tr("Export Records"),
                                                    suggestedFilePath);

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

void MainWindow::treeViewSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected)
{
    Q_UNUSED(deselected);

    if (selected.empty())
    {
        this->resetFields();
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

    int index = findInsertionIndex(record.fieldValues.keys(), fieldId, qStringLessThanLowerCase);

    // Update model.
    this->controller->getRecordsController().addRecordField(record.id, fieldId);

    // Update view.
    this->ui->tableWidget->insertRow(index);
    this->updateRecordRow(index);
}

void MainWindow::resetRecords()
{
    this->ui->treeWidget->setColumnCount(1);

    while (this->ui->treeWidget->topLevelItemCount() > 0)
    {
        this->ui->treeWidget->takeTopLevelItem(0);
    }
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

void MainWindow::openProject(QString path)
{
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
    QString recordDisplayName = this->getSelectedRecordDisplayName();
    const Record& record =
            this->controller->getRecordsController().getRecordByDisplayName(recordDisplayName);

    // Update model.
    this->controller->getRecordsController().removeRecordField(record.id, fieldId);

    // Update view.
    int index = findInsertionIndex(record.fieldValues.keys(), fieldId, qStringLessThanLowerCase);
    this->ui->tableWidget->removeRow(index);
}

void MainWindow::onProjectChanged()
{
    // Enable project-specific buttons.
    this->updateMenus();

    // Setup tree view.
    this->resetRecords();

    QList<QTreeWidgetItem *> items;

    const RecordSetList& recordSetList = this->controller->getRecordsController().getRecordSets();

    for (int i = 0; i < recordSetList.size(); ++i)
    {
        const RecordSet& recordSet = recordSetList[i];

        for (int j = 0; j < recordSet.records.size(); ++j)
        {
            const Record& record = recordSet.records[j];
            items.append(new QTreeWidgetItem((QTreeWidget*)0, QStringList(record.displayName)));
        }
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

void MainWindow::resetFields()
{
    this->ui->tableWidget->clear();

    this->ui->tableWidget->setRowCount(0);
    this->ui->tableWidget->setColumnCount(2);

    QStringList headers;
    headers << tr("Field");
    headers << tr("Value");
    this->ui->tableWidget->setHorizontalHeaderLabels(headers);
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
    // Get selected record.
    QString selectedRecordDisplayName = this->getSelectedRecordDisplayName();
    const Record& record =
            this->controller->getRecordsController().getRecordByDisplayName(selectedRecordDisplayName);

    // Get selected record field key and value.
    QString key = record.fieldValues.keys()[i];
    QVariant value = record.fieldValues[key];

    QString valueString = value.toString();

    // Get selected record field type.
    const FieldDefinition& field =
            this->controller->getFieldDefinitionsController().getFieldDefinition(key);

    if (this->controller->getTypesController().isCustomType(field.fieldType))
    {
        const CustomType& customType = this->controller->getTypesController().getCustomType(field.fieldType);

        if (customType.isList())
        {
            valueString = toString(value.toList());
        }
    }

    // Show field and value.
    this->ui->tableWidget->setItem(i, 0, new QTableWidgetItem(key));
    this->ui->tableWidget->setItem(i, 1, new QTableWidgetItem(valueString));

    // Show field description as tooltip.
    this->ui->tableWidget->item(i, 0)->setData(Qt::ToolTipRole, field.description);
    this->ui->tableWidget->item(i, 1)->setData(Qt::ToolTipRole, field.description);

    // Show color preview.
    if (field.fieldType == BuiltInType::Color)
    {
        QColor color = value.value<QColor>();
        this->ui->tableWidget->item(i, 1)->setData(Qt::DecorationRole, color);
    }
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
