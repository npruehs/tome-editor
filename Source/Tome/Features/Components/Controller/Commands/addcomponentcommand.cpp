#include "addcomponentcommand.h"

#include "../componentscontroller.h"

using namespace Tome;

AddComponentCommand::AddComponentCommand(ComponentsController& componentsController, const QString& componentName, const QString& componentSetName) :
    componentsController(componentsController),
    componentName(componentName),
    componentSetName(componentSetName)
{
    this->setText(tr("Add Component - %1").arg(componentName));
}

void AddComponentCommand::undo()
{
    this->componentsController.removeComponent(this->component);
}

void AddComponentCommand::redo()
{
    this->component = this->componentsController.addComponent(this->componentName, this->componentSetName);
}
