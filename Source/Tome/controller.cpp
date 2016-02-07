#include "controller.h"

#include "Components/Controller/componentscontroller.h"

using namespace Tome;


Controller::Controller() :
    componentsController(QSharedPointer<ComponentsController>::create())
{
}

QSharedPointer<ComponentsController> Controller::getComponentsController()
{
    return this->componentsController;
}

void Controller::setProject(QSharedPointer<Project> project)
{
    this->componentsController->setComponents(project->components);
}
