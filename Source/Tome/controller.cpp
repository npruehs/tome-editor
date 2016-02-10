#include "controller.h"

#include "Components/Controller/componentscontroller.h"
#include "Export/Controller/exportcontroller.h"
#include "Fields/Controller/fielddefinitionscontroller.h"
#include "Records/Controller/recordscontroller.h"

using namespace Tome;


Controller::Controller() :
    componentsController(new ComponentsController()),
    fieldDefinitionsController(new FieldDefinitionsController()),
    recordsController(new RecordsController()),
    exportController(new ExportController(*this->fieldDefinitionsController, *this->recordsController))
{
}

Controller::~Controller()
{
    delete this->componentsController;
    delete this->fieldDefinitionsController;
    delete this->recordsController;
    delete this->exportController;
}

ComponentsController& Controller::getComponentsController()
{
    return *this->componentsController;
}

FieldDefinitionsController&Controller::getFieldDefinitionsController()
{
    return *this->fieldDefinitionsController;
}

RecordsController&Controller::getRecordsController()
{
    return *this->recordsController;
}

ExportController&Controller::getExportController()
{
    return *this->exportController;
}

void Controller::setProject(QSharedPointer<Project> project)
{
    this->componentsController->setComponents(project->components);
    this->fieldDefinitionsController->setFieldDefinitionSets(project->fieldDefinitionSets);
    this->recordsController->setRecordSets(project->recordSets);
}
