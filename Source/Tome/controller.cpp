#include "controller.h"

#include "Components/Controller/componentscontroller.h"

using namespace Tome;


Controller::Controller() :
    componentsController(new ComponentsController())
{
}

Controller::~Controller()
{
    delete this->componentsController;
}

ComponentsController& Controller::getComponentsController()
{
    return *this->componentsController;
}

void Controller::setProject(QSharedPointer<Project> project)
{
    this->componentsController->setComponents(project->components);
}
