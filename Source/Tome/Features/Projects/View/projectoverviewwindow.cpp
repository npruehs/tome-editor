#include "projectoverviewwindow.h"
#include "ui_projectoverviewwindow.h"

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
    ui->setupUi(this);
}

ProjectOverviewWindow::~ProjectOverviewWindow()
{
    delete ui;
}

void ProjectOverviewWindow::showEvent(QShowEvent* event)
{
    QDialog::showEvent(event);

    // Update project data.
    this->ui->labelProjectPathValue->setText(this->controller->getFullProjectPath());

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
    RecordsController& recordsController = this->controller->getRecordsController();
    const int recordSetCount = recordsController.getRecordSets().count();
    const int recordCount = recordsController.getRecords().count();
    const QString recordsText = QString(tr("%1 record%2 (in %3 file%4)")).arg(
                QString::number(recordCount),
                recordCount != 1 ? "s" : "",
                QString::number(recordSetCount),
                recordSetCount != 1 ? "s" : "");
    this->ui->labelRecordsValue->setText(recordsText);

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
