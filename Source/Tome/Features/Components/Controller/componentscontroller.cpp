#include "componentscontroller.h"

#include "../../../Util/listutils.h"


using namespace Tome;


ComponentsController::ComponentsController()
{
}

const Component ComponentsController::addComponent(const QString& componentName)
{
    Component component = Component(componentName);
    int index = findInsertionIndex(*this->model, component);
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

void ComponentsController::removeComponentAt(const int index)
{
    this->model->removeAt(index);
}

void ComponentsController::setComponents(ComponentList& model)
{
    this->model = &model;
}
