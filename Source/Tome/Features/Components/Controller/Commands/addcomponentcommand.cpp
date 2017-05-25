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
    qInfo(qUtf8Printable(QString("Undo add component %1.").arg(this->componentName)));

    this->componentsController.removeComponent(this->component);
}

void AddComponentCommand::redo()
{
    this->component = this->componentsController.addComponent(this->componentName, this->componentSetName);
}
