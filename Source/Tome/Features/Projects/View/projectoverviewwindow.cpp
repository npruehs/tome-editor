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
    this->ui->verticalLayoutRecordsButtons->setAlignment(Qt::AlignTop);

    // Connect signals.
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
    ComponentsController& componentsController = this->controller->getComponentsController();
    const int componentSetCount = componentsController.getComponentSets().count();
    const int componentCount = componentsController.getComponents().count();
    const QString componentsText = QString(tr("%1 component%2 (in %3 file%4)")).arg(
                QString::number(componentCount),
                componentCount != 1 ? "s" : "",
                QString::number(componentSetCount),
                componentSetCount != 1 ? "s" : "");
    this->ui->labelComponentsValue->setText(componentsText);

    // Update export data.
    ExportController& exportController = this->controller->getExportController();
    const int exportTemplateCount = exportController.getRecordExportTemplates().count();
    const QString exportTemplatesText = QString(tr("%1 export template%2")).arg(
                QString::number(exportTemplateCount),
                exportTemplateCount != 1 ? "s" : "");
    this->ui->labelExportTemplatesValue->setText(exportTemplatesText);

    // Update fields data.
    FieldDefinitionsController& fieldDefintionsController = this->controller->getFieldDefinitionsController();
    const int fieldDefinitionSetCount = fieldDefintionsController.getFieldDefinitionSets().count();
    const int fieldDefintionCount = fieldDefintionsController.getFieldDefinitions().count();
    const QString fieldsText = QString(tr("%1 field%2 (in %3 file%4)")).arg(
                QString::number(fieldDefintionCount),
                fieldDefintionCount != 1 ? "s" : "",
                QString::number(fieldDefinitionSetCount),
                fieldDefinitionSetCount != 1 ? "s" : "");
    this->ui->labelFieldsValue->setText(fieldsText);

    // Update record data.
    this->updateRecordData();

    // Update type data.
    TypesController& typesController = this->controller->getTypesController();
    const int typeSetCount = typesController.getCustomTypeSets().count();
    const int typeCount = typesController.getCustomTypes().count();
    const QString typesText = QString(tr("%1 type%2 (in %3 file%4)")).arg(
                QString::number(typeCount),
                typeCount != 1 ? "s" : "",
                QString::number(typeSetCount),
                typeSetCount != 1 ? "s" : "");
    this->ui->labelCustomTypesValue->setText(typesText);
}

QListWidgetItem* ProjectOverviewWindow::getSelectedRecordSet()
{
    QList<QListWidgetItem*> selectedItems = this->ui->listWidgetRecords->selectedItems();

    if (selectedItems.empty())
    {
        return nullptr;
    }

    return selectedItems.first();
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

void ProjectOverviewWindow::onNavigateToRecordsFileClicked(bool checked)
{
    Q_UNUSED(checked)

    // Get selected record set.
    QListWidgetItem* selectedRecordSetItem = this->getSelectedRecordSet();

    if (selectedRecordSetItem == nullptr)
    {
        return;
    }

    // Get record set path.
    const QString& recordSetPath = selectedRecordSetItem->data(Qt::DisplayRole).toString();
    QFileInfo recordFile(recordSetPath);

    // Open in explorer or finder.
    QDesktopServices::openUrl(QUrl(recordFile.absolutePath()));
}

void ProjectOverviewWindow::onRemoveRecordsFileClicked(bool checked)
{
    Q_UNUSED(checked)

    // Get selected record set.
    QListWidgetItem* selectedRecordSetItem = this->getSelectedRecordSet();

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
