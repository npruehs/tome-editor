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
#include "../../Records/Controller/recordscontroller.h"
#include "../../Types/Controller/typescontroller.h"
#include "../../../Core/controller.h"

using namespace Tome;

ProjectOverviewWindow::ProjectOverviewWindow(Controller* controller, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProjectOverviewWindow),
    controller(controller)
{
    this->ui->setupUi(this);

    // Align buttons.
    this->ui->verticalLayoutComponentsButtons->setAlignment(Qt::AlignTop);
    this->ui->verticalLayoutCustomTypesButtons->setAlignment(Qt::AlignTop);
    this->ui->verticalLayoutFieldsButtons->setAlignment(Qt::AlignTop);
    this->ui->verticalLayoutRecordsButtons->setAlignment(Qt::AlignTop);

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

    // Update component data.
    this->updateComponentData();

    // Update export data.
    ExportController& exportController = this->controller->getExportController();
    const int exportTemplateCount = exportController.getRecordExportTemplates().count();
    const QString exportTemplatesText = QString(tr("%1 export template%2")).arg(
                QString::number(exportTemplateCount),
                exportTemplateCount != 1 ? "s" : "");
    this->ui->labelExportTemplatesValue->setText(exportTemplatesText);

    // Update fields data.
    this->updateFieldDefinitionData();

    // Update record data.
    this->updateRecordData();

    // Update type data.
    this->updateCustomTypeData();
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
    QFileInfo file(filePath);

    // Open in explorer or finder.
    QDesktopServices::openUrl(QUrl(file.absolutePath()));
}

void ProjectOverviewWindow::updateComponentData()
{
    // Count component sets and components.
    ComponentsController& componentsController = this->controller->getComponentsController();
    const ComponentSetList& componentSets = componentsController.getComponentSets();
    const int componentSetCount = componentSets.count();
    const int componentCount = componentsController.getComponents().count();
    const QString componentsText = QString(tr("%1 component%2 (in %3 file%4)")).arg(
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
        const QString& componentSetPath = this->controller->buildFullFilePath(
                    componentSet.name,
                    this->controller->getProjectPath(),
                    Controller::ComponentFileExtension);

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
    TypesController& typesController = this->controller->getTypesController();
    const CustomTypeSetList& customTypeSets = typesController.getCustomTypeSets();
    const int typeSetCount = customTypeSets.count();
    const int typeCount = typesController.getCustomTypes().count();
    const QString typesText = QString(tr("%1 type%2 (in %3 file%4)")).arg(
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
        const QString& customTypeSetPath = this->controller->buildFullFilePath(
                    customTypeSet.name,
                    this->controller->getProjectPath(),
                    Controller::TypeFileExtension);

        // Add list item.
        QListWidgetItem* item = new QListWidgetItem();
        item->setData(Qt::DisplayRole, customTypeSetPath);
        item->setData(Qt::UserRole, customTypeSet.name);
        this->ui->listWidgetCustomTypes->addItem(item);
    }
}

void ProjectOverviewWindow::updateFieldDefinitionData()
{
    // Count field definition sets and field definitions.
    FieldDefinitionsController& fieldDefintionsController = this->controller->getFieldDefinitionsController();
    const FieldDefinitionSetList& fieldDefinitionSets = fieldDefintionsController.getFieldDefinitionSets();
    const int fieldDefinitionSetCount = fieldDefinitionSets.count();
    const int fieldDefintionCount = fieldDefintionsController.getFieldDefinitions().count();
    const QString fieldsText = QString(tr("%1 field%2 (in %3 file%4)")).arg(
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
        const QString& fieldDefinitionSetPath = this->controller->buildFullFilePath(
                    fieldDefinitionSet.name,
                    this->controller->getProjectPath(),
                    Controller::FieldDefinitionFileExtension);

        // Add list item.
        QListWidgetItem* item = new QListWidgetItem();
        item->setData(Qt::DisplayRole, fieldDefinitionSetPath);
        item->setData(Qt::UserRole, fieldDefinitionSet.name);
        this->ui->listWidgetFields->addItem(item);
    }
}

void ProjectOverviewWindow::updateRecordData()
{
    // Count records sets and records.
    RecordsController& recordsController = this->controller->getRecordsController();
    const RecordSetList& recordSets = recordsController.getRecordSets();
    const int recordSetCount = recordSets.count();
    const int recordCount = recordsController.getRecords().count();
    const QString recordsText = QString(tr("%1 record%2 (in %3 file%4)")).arg(
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
        const QString& recordSetPath = this->controller->buildFullFilePath(
                    recordSet.name,
                    this->controller->getProjectPath(),
                    Controller::RecordFileExtension);

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
    const QString& projectPath = this->controller->getProjectPath();
    const QString& componentSetFileName = QFileDialog::getOpenFileName(this,
                                                                  tr("Add Existing Components File"),
                                                                  projectPath,
                                                                  "Tome Component Files (*.tcomp)");
    QDir projectDirectory = QDir(projectPath);
    const QString& relativeComponentFilePath = projectDirectory.relativeFilePath(componentSetFileName);

    try
    {
        // Load components.
        ComponentSet componentSet = ComponentSet();
        componentSet.name = relativeComponentFilePath;
        this->controller->loadComponentSet(projectPath, componentSet);

        // Update model.
        this->controller->getComponentsController().addComponentSet(componentSet);

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
    const QString& projectPath = this->controller->getProjectPath();
    const QString& customTypeSetFileName = QFileDialog::getOpenFileName(this,
                                                                  tr("Add Existing Types File"),
                                                                  projectPath,
                                                                  "Tome Type Files (*.ttypes)");
    QDir projectDirectory = QDir(projectPath);
    const QString& relativeCustomTypeFilePath = projectDirectory.relativeFilePath(customTypeSetFileName);

    try
    {
        // Load custom types.
        CustomTypeSet customTypeSet = CustomTypeSet();
        customTypeSet.name = relativeCustomTypeFilePath;
        this->controller->loadCustomTypeSet(projectPath, customTypeSet);

        // Update model.
        this->controller->getTypesController().addCustomTypeSet(customTypeSet);

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

void ProjectOverviewWindow::onAddExistingFieldDefinitionsFileClicked(bool checked)
{
    Q_UNUSED(checked)

    // Open file browser dialog.
    const QString& projectPath = this->controller->getProjectPath();
    const QString& fieldDefinitionSetFileName = QFileDialog::getOpenFileName(this,
                                                                  tr("Add Existing Field Definitions File"),
                                                                  projectPath,
                                                                  "Tome Field Definition Files (*.tfields)");
    QDir projectDirectory = QDir(projectPath);
    const QString& relativeFieldDefinitionFilePath = projectDirectory.relativeFilePath(fieldDefinitionSetFileName);

    try
    {
        // Load field definitions.
        FieldDefinitionSet fieldDefinitionSet = FieldDefinitionSet();
        fieldDefinitionSet.name = relativeFieldDefinitionFilePath;
        this->controller->loadFieldDefinitionSet(projectPath, fieldDefinitionSet);

        // Update model.
        this->controller->getFieldDefinitionsController().addFieldDefinitionSet(fieldDefinitionSet);

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

void ProjectOverviewWindow::onAddExistingRecordsFileClicked(bool checked)
{
    Q_UNUSED(checked)

    // Open file browser dialog.
    const QString& projectPath = this->controller->getProjectPath();
    const QString& recordSetFileName = QFileDialog::getOpenFileName(this,
                                                                  tr("Add Existing Records File"),
                                                                  projectPath,
                                                                  "Tome Record Files (*.tdata)");
    QDir projectDirectory = QDir(projectPath);
    const QString& relativeRecordFilePath = projectDirectory.relativeFilePath(recordSetFileName);

    try
    {
        // Load records.
        RecordSet recordSet = RecordSet();
        recordSet.name = relativeRecordFilePath;
        this->controller->loadRecordSet(projectPath, recordSet);

        // Update model.
        this->controller->getRecordsController().addRecordSet(recordSet);

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
    const QString& projectPath = this->controller->getProjectPath();
    const QString& componentSetFileName = QFileDialog::getSaveFileName(this,
                                                                  tr("Add New Components File"),
                                                                  projectPath,
                                                                  "Tome Component Files (*.tcomp)");
    QDir projectDirectory = QDir(projectPath);
    const QString& relativeComponentFilePath = projectDirectory.relativeFilePath(componentSetFileName);

    // Create new component set.
    ComponentSet componentSet = ComponentSet();
    componentSet.name = relativeComponentFilePath;

    // Update model.
    this->controller->getComponentsController().addComponentSet(componentSet);

    // Update view.
    this->updateComponentData();
}

void ProjectOverviewWindow::onAddNewCustomTypesFileClicked(bool checked)
{
    Q_UNUSED(checked)

    // Open file browser dialog.
    const QString& projectPath = this->controller->getProjectPath();
    const QString& customTypeSetFileName = QFileDialog::getSaveFileName(this,
                                                                  tr("Add New Types File"),
                                                                  projectPath,
                                                                  "Tome Type Files (*.ttypes)");
    QDir projectDirectory = QDir(projectPath);
    const QString& relativeCustomTypeFilePath = projectDirectory.relativeFilePath(customTypeSetFileName);

    // Create new custom type set.
    CustomTypeSet customTypeSet = CustomTypeSet();
    customTypeSet.name = relativeCustomTypeFilePath;

    // Update model.
    this->controller->getTypesController().addCustomTypeSet(customTypeSet);

    // Update view.
    this->updateCustomTypeData();
}

void ProjectOverviewWindow::onAddNewFieldDefinitionsFileClicked(bool checked)
{
    Q_UNUSED(checked)

    // Open file browser dialog.
    const QString& projectPath = this->controller->getProjectPath();
    const QString& fieldDefinitionSetFileName = QFileDialog::getSaveFileName(this,
                                                                  tr("Add New Field Definitions File"),
                                                                  projectPath,
                                                                  "Tome Field Definition Files (*.tfields)");
    QDir projectDirectory = QDir(projectPath);
    const QString& relativeFieldDefinitionFilePath = projectDirectory.relativeFilePath(fieldDefinitionSetFileName);

    // Create new field definition set.
    FieldDefinitionSet fieldDefinitionSet = FieldDefinitionSet();
    fieldDefinitionSet.name = relativeFieldDefinitionFilePath;

    // Update model.
    this->controller->getFieldDefinitionsController().addFieldDefinitionSet(fieldDefinitionSet);

    // Update view.
    this->updateFieldDefinitionData();
}

void ProjectOverviewWindow::onAddNewRecordsFileClicked(bool checked)
{
    Q_UNUSED(checked)

    // Open file browser dialog.
    const QString& projectPath = this->controller->getProjectPath();
    const QString& recordSetFileName = QFileDialog::getSaveFileName(this,
                                                                  tr("Add New Records File"),
                                                                  projectPath,
                                                                  "Tome Record Files (*.tdata)");
    QDir projectDirectory = QDir(projectPath);
    const QString& relativeRecordFilePath = projectDirectory.relativeFilePath(recordSetFileName);

    // Create new record set.
    RecordSet recordSet = RecordSet();
    recordSet.name = relativeRecordFilePath;

    // Update model.
    this->controller->getRecordsController().addRecordSet(recordSet);

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

void ProjectOverviewWindow::onNavigateToFieldDefinitionsFileClicked(bool checked)
{
    Q_UNUSED(checked)

    QListWidgetItem* selectedFieldDefinitionSetItem = this->getSelectedListWidgetItem(this->ui->listWidgetFields);
    this->navigateToSelectedFile(selectedFieldDefinitionSetItem);
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
    this->controller->getComponentsController().removeComponentSet(componentSetName);

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
    this->controller->getTypesController().removeCustomTypeSet(customTypeSetName);

    // Update view.
    this->updateCustomTypeData();
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
    this->controller->getFieldDefinitionsController().removeFieldDefinitionSet(fieldDefinitionSetName);

    // Update view.
    this->updateFieldDefinitionData();
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
    this->controller->getRecordsController().removeRecordSet(recordSetName);

    // Update view.
    this->updateRecordData();
}
