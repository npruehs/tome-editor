#include "componentscontroller.h"

#include "../../../Util/listutils.h"
#include "../../../Util/stringutils.h"


using namespace Tome;


ComponentsController::ComponentsController()
{
}

const Component ComponentsController::addComponent(const QString& componentName)
{
    Component component = Component(componentName);
    int index = findInsertionIndex(*this->model, component, qStringLessThanLowerCase);
    this->model->insert(index, component);
    return component;
}

const ComponentList& ComponentsController::getComponents() const
{
    return *this->model;
}

int ComponentsController::indexOf(const Component& component) const
{
    return this->model->indexOf(component);
}

void ComponentsController::removeComponent(const Component component)
{
    int index = this->model->indexOf(component);
    this->model->removeAt(index);
}

void ComponentsController::setComponents(ComponentList& model)
{
    this->model = &model;
}
