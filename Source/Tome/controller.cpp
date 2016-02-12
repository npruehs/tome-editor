#include "controller.h"

#include "Components/Controller/componentscontroller.h"
#include "Export/Controller/exportcontroller.h"
#include "Fields/Controller/fielddefinitionscontroller.h"
#include "Records/Controller/recordscontroller.h"
#include "Settings/Controller/settingscontroller.h"
#include "Types/Controller/typescontroller.h"

using namespace Tome;


Controller::Controller() :
    componentsController(new ComponentsController()),
    fieldDefinitionsController(new FieldDefinitionsController()),
    recordsController(new RecordsController(*this->fieldDefinitionsController)),
    exportController(new ExportController(*this->fieldDefinitionsController, *this->recordsController)),
    settingsController(new SettingsController()),
    typesController(new TypesController())
{
}

Controller::~Controller()
{
    delete this->componentsController;
    delete this->fieldDefinitionsController;
    delete this->recordsController;
    delete this->exportController;
    delete this->settingsController;
    delete this->typesController;
}

ComponentsController& Controller::getComponentsController()
{
    return *this->componentsController;
}

FieldDefinitionsController& Controller::getFieldDefinitionsController()
{
    return *this->fieldDefinitionsController;
}

RecordsController& Controller::getRecordsController()
{
    return *this->recordsController;
}

ExportController& Controller::getExportController()
{
    return *this->exportController;
}

SettingsController& Controller::getSettingsController()
{
    return *this->settingsController;
}

TypesController& Controller::getTypesController()
{
    return *this->typesController;
}

void Controller::setProject(QSharedPointer<Project> project)
{
    this->componentsController->setComponents(project->components);
    this->fieldDefinitionsController->setFieldDefinitionSets(project->fieldDefinitionSets);
    this->recordsController->setRecordSets(project->recordSets);
    this->typesController->setCustomTypes(project->types);
}
