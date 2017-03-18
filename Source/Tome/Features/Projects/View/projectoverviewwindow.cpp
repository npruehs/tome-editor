#include "projectoverviewwindow.h"
#include "ui_projectoverviewwindow.h"

#include <QDesktopServices>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QUrl>

#include "../../Components/Controller/componentscontroller.h"
#include "../../Export/Controller/exportcontroller.h"
#include "../../Fields/Controller/fielddefinitionscontroller.h"
#include "../../Import/Controller/importcontroller.h"
#include "../../Projects/Controller/projectcontroller.h"
#include "../../Records/Controller/recordscontroller.h"
#include "../../Types/Controller/typescontroller.h"
#include "../../../Util/pathutils.h"

using namespace Tome;

ProjectOverviewWindow::ProjectOverviewWindow(ProjectController& projectController,
                                             ComponentsController& componentsController,
                                             ExportController& exportController,
                                             FieldDefinitionsController& fieldDefinitionsController,
                                             ImportController& importController,
                                             RecordsController& recordsController,
                                             TypesController& typesController,
                                             QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProjectOverviewWindow),
    componentsController(componentsController),
    exportController(exportController),
    fieldDefinitionsController(fieldDefinitionsController),
    importController(importController),
    projectController(projectController),
    recordsController(recordsController),
    typesController(typesController)
{
    this->ui->setupUi(this);

    // Align buttons.
    this->ui->verticalLayoutComponentsButtons->setAlignment(Qt::AlignTop);
    this->ui->verticalLayoutCustomTypesButtons->setAlignment(Qt::AlignTop);
    this->ui->verticalLayoutExportTemplatesButtons->setAlignment(Qt::AlignTop);
    this->ui->verticalLayoutFieldsButtons->setAlignment(Qt::AlignTop);
    this->ui->verticalLayoutRecordsButtons->setAlignment(Qt::AlignTop);
    this->ui->verticalLayoutImportTemplatesButtons->setAlignment(Qt::AlignTop);

    // Connect component button signals.
    connect(
                this->ui->pushButtonAddExistingComponentsFile,
                SIGNAL(clicked(bool)),
                SLOT(onAddExistingComponentsFileClicked(bool))
                );

    connect(
                this->ui->pushButtonAddNewComponentsFile,
                SIGNAL(clicked(bool)),
                SLOT(onAddNewComponentsFileClicked(bool))
                );

    connect(
                this->ui->pushButtonNavigateToComponentsFile,
                SIGNAL(clicked(bool)),
                SLOT(onNavigateToComponentsFileClicked(bool))
                );

    connect(
                this->ui->pushButtonRemoveComponentsFile,
                SIGNAL(clicked(bool)),
                SLOT(onRemoveComponentsFileClicked(bool))
                );

    // Connect custom type button signals.
    connect(
                this->ui->pushButtonAddExistingCustomTypesFile,
                SIGNAL(clicked(bool)),
                SLOT(onAddExistingCustomTypesFileClicked(bool))
                );

    connect(
                this->ui->pushButtonAddNewCustomTypesFile,
                SIGNAL(clicked(bool)),
                SLOT(onAddNewCustomTypesFileClicked(bool))
                );

    connect(
                this->ui->pushButtonNavigateToCustomTypesFile,
                SIGNAL(clicked(bool)),
                SLOT(onNavigateToCustomTypesFileClicked(bool))
                );

    connect(
                this->ui->pushButtonRemoveCustomTypesFile,
                SIGNAL(clicked(bool)),
                SLOT(onRemoveCustomTypesFileClicked(bool))
                );

    // Connect export template button signals.
    connect(
                this->ui->pushButtonAddExistingExportTemplate,
                SIGNAL(clicked(bool)),
                SLOT(onAddExistingExportTemplateFileClicked(bool))
                );

    connect(
                this->ui->pushButtonNavigateToExportTemplate,
                SIGNAL(clicked(bool)),
                SLOT(onNavigateToExportTemplateFileClicked(bool))
                );

    connect(
                this->ui->pushButtonRemoveExportTemplate,
                SIGNAL(clicked(bool)),
                SLOT(onRemoveExportTemplateFileClicked(bool))
                );

    // Connect field definition button signals.
    connect(
                this->ui->pushButtonAddExistingFieldsFile,
                SIGNAL(clicked(bool)),
                SLOT(onAddExistingFieldDefinitionsFileClicked(bool))
                );

    connect(
                this->ui->pushButtonAddNewFieldsFile,
                SIGNAL(clicked(bool)),
                SLOT(onAddNewFieldDefinitionsFileClicked(bool))
                );

    connect(
                this->ui->pushButtonNavigateToFieldsFile,
                SIGNAL(clicked(bool)),
                SLOT(onNavigateToFieldDefinitionsFileClicked(bool))
                );

    connect(
                this->ui->pushButtonRemoveFieldsFile,
                SIGNAL(clicked(bool)),
                SLOT(onRemoveFieldDefinitionsFileClicked(bool))
                );

    // Connect import template button signals.
    connect(
                this->ui->pushButtonAddExistingImportTemplate,
                SIGNAL(clicked(bool)),
                SLOT(onAddExistingImportTemplateFileClicked(bool))
                );

    connect(
                this->ui->pushButtonNavigateToImportTemplate,
                SIGNAL(clicked(bool)),
                SLOT(onNavigateToImportTemplateFileClicked(bool))
                );

    connect(
                this->ui->pushButtonRemoveImportTemplate,
                SIGNAL(clicked(bool)),
                SLOT(onRemoveImportTemplateFileClicked(bool))
                );

    // Connect record button signals.
    connect(
                this->ui->pushButtonAddExistingRecordsFile,
                SIGNAL(clicked(bool)),
                SLOT(onAddExistingRecordsFileClicked(bool))
                );

    connect(
                this->ui->pushButtonAddNewRecordsFile,
                SIGNAL(clicked(bool)),
                SLOT(onAddNewRecordsFileClicked(bool))
                );

    connect(
                this->ui->pushButtonNavigateToRecordsFile,
                SIGNAL(clicked(bool)),
                SLOT(onNavigateToRecordsFileClicked(bool))
                );

    connect(
                this->ui->pushButtonRemoveRecordsFile,
                SIGNAL(clicked(bool)),
                SLOT(onRemoveRecordsFileClicked(bool))
                );
}

ProjectOverviewWindow::~ProjectOverviewWindow()
{
    delete ui;
}

void ProjectOverviewWindow::showEvent(QShowEvent* event)
{
    QDialog::showEvent(event);

    this->updateComponentData();
    this->updateCustomTypeData();
    this->updateExportTemplateData();
    this->updateFieldDefinitionData();
    this->updateImportTemplateData();
    this->updateRecordData();
}

QListWidgetItem*ProjectOverviewWindow::getSelectedListWidgetItem(QListWidget* listWidget)
{
    QList<QListWidgetItem*> selectedItems = listWidget->selectedItems();

    if (selectedItems.empty())
    {
        return nullptr;
    }

    return selectedItems.first();
}

void ProjectOverviewWindow::navigateToSelectedFile(QListWidgetItem* item)
{
    if (item == nullptr)
    {
        return;
    }

    // Get file path.
    const QString& filePath = item->data(Qt::DisplayRole).toString();
    showFileInExplorerOrFinder(filePath);
}

void ProjectOverviewWindow::updateComponentData()
{
    // Count component sets and components.
    const ComponentSetList& componentSets = this->componentsController.getComponentSets();
    const int componentSetCount = componentSets.count();
    const int componentCount = componentsController.getComponents().count();
    const QString componentsText = tr("%1 component%2 (in %3 file%4)").arg(
                QString::number(componentCount),
                componentCount != 1 ? "s" : "",
                QString::number(componentSetCount),
                componentSetCount != 1 ? "s" : "");
    this->ui->labelComponentsValue->setText(componentsText);

    // Add list items.
    this->ui->listWidgetComponents->clear();

    for (int i = 0; i < componentSets.count(); ++i)
    {
        const ComponentSet& componentSet = componentSets[i];
        const QString& componentSetPath = this->projectController.buildFullFilePath(
                    componentSet.name,
                    this->projectController.getProjectPath(),
                    ProjectController::ComponentFileExtension);

        // Add list item.
        QListWidgetItem* item = new QListWidgetItem();
        item->setData(Qt::DisplayRole, componentSetPath);
        item->setData(Qt::UserRole, componentSet.name);
        this->ui->listWidgetComponents->addItem(item);
    }
}

void ProjectOverviewWindow::updateCustomTypeData()
{
    // Count custom type sets and custom types.
    const CustomTypeSetList& customTypeSets = this->typesController.getCustomTypeSets();
    const int typeSetCount = customTypeSets.count();
    const int typeCount = typesController.getCustomTypes().count();
    const QString typesText = tr("%1 type%2 (in %3 file%4)").arg(
                QString::number(typeCount),
                typeCount != 1 ? "s" : "",
                QString::number(typeSetCount),
                typeSetCount != 1 ? "s" : "");
    this->ui->labelCustomTypesValue->setText(typesText);

    // Add list items.
    this->ui->listWidgetCustomTypes->clear();

    for (int i = 0; i < customTypeSets.count(); ++i)
    {
        const CustomTypeSet& customTypeSet = customTypeSets[i];
        const QString& customTypeSetPath = this->projectController.buildFullFilePath(
                    customTypeSet.name,
                    this->projectController.getProjectPath(),
                    ProjectController::TypeFileExtension);

        // Add list item.
        QListWidgetItem* item = new QListWidgetItem();
        item->setData(Qt::DisplayRole, customTypeSetPath);
        item->setData(Qt::UserRole, customTypeSet.name);
        this->ui->listWidgetCustomTypes->addItem(item);
    }
}

void ProjectOverviewWindow::updateExportTemplateData()
{
    // Count export templates.
    const RecordExportTemplateList& exportTemplateList = this->exportController.getRecordExportTemplates();
    const int exportTemplateCount = exportTemplateList.count();
    const QString exportTemplatesText = tr("%1 export template%2").arg(
                QString::number(exportTemplateCount),
                exportTemplateCount != 1 ? "s" : "");
    this->ui->labelExportTemplatesValue->setText(exportTemplatesText);

    // Add list items.
    this->ui->listWidgetExportTemplates->clear();

    for (RecordExportTemplateList::const_iterator it = exportTemplateList.cbegin();
         it != exportTemplateList.cend();
         ++it)
    {
        const RecordExportTemplate& exportTemplate = *it;
        const QString& exportTemplatePath = this->projectController.buildFullFilePath(
                    exportTemplate.path,
                    this->projectController.getProjectPath(),
                    ProjectController::RecordExportTemplateFileExtension);

        // Add list item.
        QListWidgetItem* item = new QListWidgetItem();
        item->setData(Qt::DisplayRole, exportTemplatePath);
        item->setData(Qt::UserRole, exportTemplate.name);
        this->ui->listWidgetExportTemplates->addItem(item);
    }
}

void ProjectOverviewWindow::updateFieldDefinitionData()
{
    // Count field definition sets and field definitions.
    const FieldDefinitionSetList& fieldDefinitionSets = this->fieldDefinitionsController.getFieldDefinitionSets();
    const int fieldDefinitionSetCount = fieldDefinitionSets.count();
    const int fieldDefintionCount = this->fieldDefinitionsController.getFieldDefinitions().count();
    const QString fieldsText = tr("%1 field%2 (in %3 file%4)").arg(
                QString::number(fieldDefintionCount),
                fieldDefintionCount != 1 ? "s" : "",
                QString::number(fieldDefinitionSetCount),
                fieldDefinitionSetCount != 1 ? "s" : "");
    this->ui->labelFieldsValue->setText(fieldsText);

    // Add list items.
    this->ui->listWidgetFields->clear();

    for (int i = 0; i < fieldDefinitionSets.count(); ++i)
    {
        const FieldDefinitionSet& fieldDefinitionSet = fieldDefinitionSets[i];
        const QString& fieldDefinitionSetPath = this->projectController.buildFullFilePath(
                    fieldDefinitionSet.name,
                    this->projectController.getProjectPath(),
                    ProjectController::FieldDefinitionFileExtension);

        // Add list item.
        QListWidgetItem* item = new QListWidgetItem();
        item->setData(Qt::DisplayRole, fieldDefinitionSetPath);
        item->setData(Qt::UserRole, fieldDefinitionSet.name);
        this->ui->listWidgetFields->addItem(item);
    }
}

void ProjectOverviewWindow::updateImportTemplateData()
{
    // Count import templates.
    const RecordTableImportTemplateList& importTemplateList = this->importController.getRecordTableImportTemplates();
    const int importTemplateCount = importTemplateList.count();
    const QString importTemplatesText = tr("%1 import template%2").arg(
                QString::number(importTemplateCount),
                importTemplateCount != 1 ? "s" : "");
    this->ui->labelImportTemplatesValue->setText(importTemplatesText);

    // Add list items.
    this->ui->listWidgetImportTemplates->clear();

    for (RecordTableImportTemplateList::const_iterator it = importTemplateList.cbegin();
         it != importTemplateList.cend();
         ++it)
    {
        const RecordTableImportTemplate& importTemplate = *it;
        const QString& importTemplatePath = this->projectController.buildFullFilePath(
                    importTemplate.path,
                    this->projectController.getProjectPath(),
                    ProjectController::RecordImportTemplateFileExtension);

        // Add list item.
        QListWidgetItem* item = new QListWidgetItem();
        item->setData(Qt::DisplayRole, importTemplatePath);
        item->setData(Qt::UserRole, importTemplate.name);
        this->ui->listWidgetImportTemplates->addItem(item);
    }
}

void ProjectOverviewWindow::updateRecordData()
{
    // Count records sets and records.
    const RecordSetList& recordSets = this->recordsController.getRecordSets();
    const int recordSetCount = recordSets.count();
    const int recordCount = recordsController.getRecords().count();
    const QString recordsText = tr("%1 record%2 (in %3 file%4)").arg(
                QString::number(recordCount),
                recordCount != 1 ? "s" : "",
                QString::number(recordSetCount),
                recordSetCount != 1 ? "s" : "");
    this->ui->labelRecordsValue->setText(recordsText);

    // Add list items.
    this->ui->listWidgetRecords->clear();

    for (int i = 0; i < recordSets.count(); ++i)
    {
        const RecordSet& recordSet = recordSets[i];
        const QString& recordSetPath = this->projectController.buildFullFilePath(
                    recordSet.name,
                    this->projectController.getProjectPath(),
                    ProjectController::RecordFileExtension);

        // Add list item.
        QListWidgetItem* item = new QListWidgetItem();
        item->setData(Qt::DisplayRole, recordSetPath);
        item->setData(Qt::UserRole, recordSet.name);
        this->ui->listWidgetRecords->addItem(item);
    }
}

void ProjectOverviewWindow::onAddExistingComponentsFileClicked(bool checked)
{
    Q_UNUSED(checked)

    // Open file browser dialog.
    const QString& projectPath = this->projectController.getProjectPath();
    const QString& componentSetFileName = QFileDialog::getOpenFileName(this,
                                                                  tr("Add Existing Components File"),
                                                                  projectPath,
                                                                  "Tome Component Files (*.tcomp)");

    if (componentSetFileName.isEmpty())
    {
        return;
    }

    QDir projectDirectory = QDir(projectPath);
    const QString& relativeComponentFilePath = projectDirectory.relativeFilePath(componentSetFileName);

    try
    {
        // Load components.
        ComponentSet componentSet = ComponentSet();
        componentSet.name = relativeComponentFilePath;
        this->projectController.loadComponentSet(projectPath, componentSet);

        // Update model.
        this->componentsController.addComponentSet(componentSet);

        // Update view.
        this->updateComponentData();
    }
    catch (std::runtime_error& e)
    {
        QMessageBox::critical(
                    this,
                    tr("Unable to load component file"),
                    e.what(),
                    QMessageBox::Close,
                    QMessageBox::Close);
    }
}

void ProjectOverviewWindow::onAddExistingCustomTypesFileClicked(bool checked)
{
    Q_UNUSED(checked)

    // Open file browser dialog.
    const QString& projectPath = this->projectController.getProjectPath();
    const QString& customTypeSetFileName = QFileDialog::getOpenFileName(this,
                                                                  tr("Add Existing Types File"),
                                                                  projectPath,
                                                                  "Tome Type Files (*.ttypes)");

    if (customTypeSetFileName.isEmpty())
    {
        return;
    }

    QDir projectDirectory = QDir(projectPath);
    const QString& relativeCustomTypeFilePath = projectDirectory.relativeFilePath(customTypeSetFileName);

    try
    {
        // Load custom types.
        CustomTypeSet customTypeSet = CustomTypeSet();
        customTypeSet.name = relativeCustomTypeFilePath;
        this->projectController.loadCustomTypeSet(projectPath, customTypeSet);

        // Update model.
        this->typesController.addCustomTypeSet(customTypeSet);

        // Update view.
        this->updateCustomTypeData();
    }
    catch (std::runtime_error& e)
    {
        QMessageBox::critical(
                    this,
                    tr("Unable to load type file"),
                    e.what(),
                    QMessageBox::Close,
                    QMessageBox::Close);
    }
}

void ProjectOverviewWindow::onAddExistingExportTemplateFileClicked(bool checked)
{
    Q_UNUSED(checked)

    // Open file browser dialog.
    const QString& projectPath = this->projectController.getProjectPath();
    const QString& exportTemplateFileName = QFileDialog::getOpenFileName(this,
                                                                  tr("Add Existing Export Template File"),
                                                                  projectPath,
                                                                  "Tome Export Template Files (*.texport)");

    if (exportTemplateFileName.isEmpty())
    {
        return;
    }

    QDir projectDirectory = QDir(projectPath);
    const QString& relativeExportTemplateFilePath = projectDirectory.relativeFilePath(exportTemplateFileName);

    try
    {
        // Load export template.
        RecordExportTemplate exportTemplate = RecordExportTemplate();
        exportTemplate.path = relativeExportTemplateFilePath;
        this->projectController.loadExportTemplate(projectPath, exportTemplate);

        // Update model.
        this->exportController.addRecordExportTemplate(exportTemplate);

        // Update view.
        this->updateExportTemplateData();
    }
    catch (std::runtime_error& e)
    {
        QMessageBox::critical(
                    this,
                    tr("Unable to load export template"),
                    e.what(),
                    QMessageBox::Close,
                    QMessageBox::Close);
    }
}

void ProjectOverviewWindow::onAddExistingFieldDefinitionsFileClicked(bool checked)
{
    Q_UNUSED(checked)

    // Open file browser dialog.
    const QString& projectPath = this->projectController.getProjectPath();
    const QString& fieldDefinitionSetFileName = QFileDialog::getOpenFileName(this,
                                                                  tr("Add Existing Field Definitions File"),
                                                                  projectPath,
                                                                  "Tome Field Definition Files (*.tfields)");

    if (fieldDefinitionSetFileName.isEmpty())
    {
        return;
    }

    QDir projectDirectory = QDir(projectPath);
    const QString& relativeFieldDefinitionFilePath = projectDirectory.relativeFilePath(fieldDefinitionSetFileName);

    try
    {
        // Load field definitions.
        FieldDefinitionSet fieldDefinitionSet = FieldDefinitionSet();
        fieldDefinitionSet.name = relativeFieldDefinitionFilePath;
        this->projectController.loadFieldDefinitionSet(projectPath, fieldDefinitionSet);

        // Update model.
        this->fieldDefinitionsController.addFieldDefinitionSet(fieldDefinitionSet);

        // Update view.
        this->updateFieldDefinitionData();
    }
    catch (std::runtime_error& e)
    {
        QMessageBox::critical(
                    this,
                    tr("Unable to load field definitions file"),
                    e.what(),
                    QMessageBox::Close,
                    QMessageBox::Close);
    }
}

void ProjectOverviewWindow::onAddExistingImportTemplateFileClicked(bool checked)
{
    Q_UNUSED(checked)

    // Open file browser dialog.
    const QString& projectPath = this->projectController.getProjectPath();
    const QString& importTemplateFileName = QFileDialog::getOpenFileName(this,
                                                                  tr("Add Existing Import Template File"),
                                                                  projectPath,
                                                                  "Tome Import Template Files (*.timport)");

    if (importTemplateFileName.isEmpty())
    {
        return;
    }

    QDir projectDirectory = QDir(projectPath);
    const QString& relativeImportTemplateFilePath = projectDirectory.relativeFilePath(importTemplateFileName);

    try
    {
        // Load import template.
        RecordTableImportTemplate importTemplate = RecordTableImportTemplate();
        importTemplate.path = relativeImportTemplateFilePath;
        this->projectController.loadImportTemplate(projectPath, importTemplate);

        // Update model.
        this->importController.addRecordImportTemplate(importTemplate);

        // Update view.
        this->updateImportTemplateData();
    }
    catch (std::runtime_error& e)
    {
        QMessageBox::critical(
                    this,
                    tr("Unable to load import template"),
                    e.what(),
                    QMessageBox::Close,
                    QMessageBox::Close);
    }
}

void ProjectOverviewWindow::onAddExistingRecordsFileClicked(bool checked)
{
    Q_UNUSED(checked)

    // Open file browser dialog.
    const QString& projectPath = this->projectController.getProjectPath();
    const QString& recordSetFileName = QFileDialog::getOpenFileName(this,
                                                                  tr("Add Existing Records File"),
                                                                  projectPath,
                                                                  "Tome Record Files (*.tdata)");

    if (recordSetFileName.isEmpty())
    {
        return;
    }

    QDir projectDirectory = QDir(projectPath);
    const QString& relativeRecordFilePath = projectDirectory.relativeFilePath(recordSetFileName);

    try
    {
        // Load records.
        RecordSet recordSet = RecordSet();
        recordSet.name = relativeRecordFilePath;
        this->projectController.loadRecordSet(projectPath, recordSet);

        // Update model.
        this->recordsController.addRecordSet(recordSet);

        // Update view.
        this->updateRecordData();
    }
    catch (std::runtime_error& e)
    {
        QMessageBox::critical(
                    this,
                    tr("Unable to load records file"),
                    e.what(),
                    QMessageBox::Close,
                    QMessageBox::Close);
    }
}

void ProjectOverviewWindow::onAddNewComponentsFileClicked(bool checked)
{
    Q_UNUSED(checked)

    // Open file browser dialog.
    const QString& projectPath = this->projectController.getProjectPath();
    const QString& componentSetFileName = QFileDialog::getSaveFileName(this,
                                                                  tr("Add New Components File"),
                                                                  projectPath,
                                                                  "Tome Component Files (*.tcomp)");

    if (componentSetFileName.isEmpty())
    {
        return;
    }

    QDir projectDirectory = QDir(projectPath);
    const QString& relativeComponentFilePath = projectDirectory.relativeFilePath(componentSetFileName);

    // Create new component set.
    ComponentSet componentSet = ComponentSet();
    componentSet.name = relativeComponentFilePath;

    // Update model.
    this->componentsController.addComponentSet(componentSet);

    // Update view.
    this->updateComponentData();
}

void ProjectOverviewWindow::onAddNewCustomTypesFileClicked(bool checked)
{
    Q_UNUSED(checked)

    // Open file browser dialog.
    const QString& projectPath = this->projectController.getProjectPath();
    const QString& customTypeSetFileName = QFileDialog::getSaveFileName(this,
                                                                  tr("Add New Types File"),
                                                                  projectPath,
                                                                  "Tome Type Files (*.ttypes)");

    if (customTypeSetFileName.isEmpty())
    {
        return;
    }

    QDir projectDirectory = QDir(projectPath);
    const QString& relativeCustomTypeFilePath = projectDirectory.relativeFilePath(customTypeSetFileName);

    // Create new custom type set.
    CustomTypeSet customTypeSet = CustomTypeSet();
    customTypeSet.name = relativeCustomTypeFilePath;

    // Update model.
    this->typesController.addCustomTypeSet(customTypeSet);

    // Update view.
    this->updateCustomTypeData();
}

void ProjectOverviewWindow::onAddNewFieldDefinitionsFileClicked(bool checked)
{
    Q_UNUSED(checked)

    // Open file browser dialog.
    const QString& projectPath = this->projectController.getProjectPath();
    const QString& fieldDefinitionSetFileName = QFileDialog::getSaveFileName(this,
                                                                  tr("Add New Field Definitions File"),
                                                                  projectPath,
                                                                  "Tome Field Definition Files (*.tfields)");

    if (fieldDefinitionSetFileName.isEmpty())
    {
        return;
    }

    QDir projectDirectory = QDir(projectPath);
    const QString& relativeFieldDefinitionFilePath = projectDirectory.relativeFilePath(fieldDefinitionSetFileName);

    // Create new field definition set.
    FieldDefinitionSet fieldDefinitionSet = FieldDefinitionSet();
    fieldDefinitionSet.name = relativeFieldDefinitionFilePath;

    // Update model.
    this->fieldDefinitionsController.addFieldDefinitionSet(fieldDefinitionSet);

    // Update view.
    this->updateFieldDefinitionData();
}

void ProjectOverviewWindow::onAddNewRecordsFileClicked(bool checked)
{
    Q_UNUSED(checked)

    // Open file browser dialog.
    const QString& projectPath = this->projectController.getProjectPath();
    const QString& recordSetFileName = QFileDialog::getSaveFileName(this,
                                                                  tr("Add New Records File"),
                                                                  projectPath,
                                                                  "Tome Record Files (*.tdata)");

    if (recordSetFileName.isEmpty())
    {
        return;
    }

    QDir projectDirectory = QDir(projectPath);
    const QString& relativeRecordFilePath = projectDirectory.relativeFilePath(recordSetFileName);

    // Create new record set.
    RecordSet recordSet = RecordSet();
    recordSet.name = relativeRecordFilePath;

    // Update model.
    this->recordsController.addRecordSet(recordSet);

    // Update view.
    this->updateRecordData();
}

void ProjectOverviewWindow::onNavigateToComponentsFileClicked(bool checked)
{
    Q_UNUSED(checked)

    QListWidgetItem* selectedComponentSetItem = this->getSelectedListWidgetItem(this->ui->listWidgetComponents);
    this->navigateToSelectedFile(selectedComponentSetItem);
}

void ProjectOverviewWindow::onNavigateToCustomTypesFileClicked(bool checked)
{
    Q_UNUSED(checked)

    QListWidgetItem* selectedCustomTypeSetItem = this->getSelectedListWidgetItem(this->ui->listWidgetCustomTypes);
    this->navigateToSelectedFile(selectedCustomTypeSetItem);
}

void ProjectOverviewWindow::onNavigateToExportTemplateFileClicked(bool checked)
{
    Q_UNUSED(checked)

    QListWidgetItem* selectedExportTemplateItem = this->getSelectedListWidgetItem(this->ui->listWidgetExportTemplates);
    this->navigateToSelectedFile(selectedExportTemplateItem);
}

void ProjectOverviewWindow::onNavigateToFieldDefinitionsFileClicked(bool checked)
{
    Q_UNUSED(checked)

    QListWidgetItem* selectedFieldDefinitionSetItem = this->getSelectedListWidgetItem(this->ui->listWidgetFields);
    this->navigateToSelectedFile(selectedFieldDefinitionSetItem);
}

void ProjectOverviewWindow::onNavigateToImportTemplateFileClicked(bool checked)
{
    Q_UNUSED(checked)

    QListWidgetItem* selectedImportTemplateItem = this->getSelectedListWidgetItem(this->ui->listWidgetImportTemplates);
    this->navigateToSelectedFile(selectedImportTemplateItem);
}

void ProjectOverviewWindow::onNavigateToRecordsFileClicked(bool checked)
{
    Q_UNUSED(checked)

    QListWidgetItem* selectedRecordSetItem = this->getSelectedListWidgetItem(this->ui->listWidgetRecords);
    this->navigateToSelectedFile(selectedRecordSetItem);
}

void ProjectOverviewWindow::onRemoveComponentsFileClicked(bool checked)
{
    Q_UNUSED(checked)

    // Get selected component set.
    QListWidgetItem* selectedComponentSetItem = this->getSelectedListWidgetItem(this->ui->listWidgetComponents);

    if (selectedComponentSetItem == nullptr)
    {
        return;
    }

    // Update model.
    const QString& componentSetName = selectedComponentSetItem->data(Qt::UserRole).toString();
    this->componentsController.removeComponentSet(componentSetName);

    // Update view.
    this->updateComponentData();
}

void ProjectOverviewWindow::onRemoveCustomTypesFileClicked(bool checked)
{
    Q_UNUSED(checked)

    // Get selected custom type set.
    QListWidgetItem* selectedCustomTypeSetItem = this->getSelectedListWidgetItem(this->ui->listWidgetCustomTypes);

    if (selectedCustomTypeSetItem == nullptr)
    {
        return;
    }

    // Update model.
    const QString& customTypeSetName = selectedCustomTypeSetItem->data(Qt::UserRole).toString();
    this->typesController.removeCustomTypeSet(customTypeSetName);

    // Update view.
    this->updateCustomTypeData();
}

void ProjectOverviewWindow::onRemoveExportTemplateFileClicked(bool checked)
{
    Q_UNUSED(checked)

    // Get selected export template.
    QListWidgetItem* selectedExportTemplateItem = this->getSelectedListWidgetItem(this->ui->listWidgetExportTemplates);

    if (selectedExportTemplateItem == nullptr)
    {
        return;
    }

    // Update model.
    const QString& exportTemplateName = selectedExportTemplateItem->data(Qt::UserRole).toString();
    this->exportController.removeExportTemplate(exportTemplateName);

    // Update view.
    this->updateExportTemplateData();
}

void ProjectOverviewWindow::onRemoveFieldDefinitionsFileClicked(bool checked)
{
    Q_UNUSED(checked)

    // Get selected field definition set.
    QListWidgetItem* selectedFieldDefinitionSetItem = this->getSelectedListWidgetItem(this->ui->listWidgetFields);

    if (selectedFieldDefinitionSetItem == nullptr)
    {
        return;
    }

    // Update model.
    const QString& fieldDefinitionSetName = selectedFieldDefinitionSetItem->data(Qt::UserRole).toString();
    this->fieldDefinitionsController.removeFieldDefinitionSet(fieldDefinitionSetName);

    // Update view.
    this->updateFieldDefinitionData();
}

void ProjectOverviewWindow::onRemoveImportTemplateFileClicked(bool checked)
{
    Q_UNUSED(checked)

    // Get selected import template.
    QListWidgetItem* selectedImportTemplateItem = this->getSelectedListWidgetItem(this->ui->listWidgetImportTemplates);

    if (selectedImportTemplateItem == nullptr)
    {
        return;
    }

    // Update model.
    const QString& importTemplateName = selectedImportTemplateItem->data(Qt::UserRole).toString();
    this->importController.removeImportTemplate(importTemplateName);

    // Update view.
    this->updateImportTemplateData();
}

void ProjectOverviewWindow::onRemoveRecordsFileClicked(bool checked)
{
    Q_UNUSED(checked)

    // Get selected record set.
    QListWidgetItem* selectedRecordSetItem = this->getSelectedListWidgetItem(this->ui->listWidgetRecords);

    if (selectedRecordSetItem == nullptr)
    {
        return;
    }

    // Update model.
    const QString& recordSetName = selectedRecordSetItem->data(Qt::UserRole).toString();
    this->recordsController.removeRecordSet(recordSetName);

    // Update view.
    this->updateRecordData();
}
