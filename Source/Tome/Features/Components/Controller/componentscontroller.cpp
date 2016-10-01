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
    ComponentList& components = (*this->model)[0].components;
    int index = findInsertionIndex(components, component, qStringLessThanLowerCase);
    components.insert(index, component);
    return component;
}

void ComponentsController::addComponentSet(const ComponentSet& componentSet)
{
    this->model->push_back(componentSet);
}

const ComponentList ComponentsController::getComponents() const
{
    ComponentList components;

    for (int i = 0; i < this->model->size(); ++i)
    {
        const ComponentSet& componentSet = this->model->at(i);

        for (int j = 0; j < componentSet.components.size(); ++j)
        {
            components << componentSet.components[j];
        }
    }

    return components;
}

const ComponentSetList& ComponentsController::getComponentSets() const
{
    return *this->model;
}

int ComponentsController::indexOf(const Component& component) const
{
    return this->model->at(0).components.indexOf(component);
}

void ComponentsController::removeComponent(const Component component)
{
    ComponentList& components = (*this->model)[0].components;

    for (ComponentList::iterator it = components.begin();
         it != components.end();
         ++it)
    {
        if (*it == component)
        {
            components.erase(it);
            return;
        }
    }
}

void ComponentsController::removeComponentSet(const QString& name)
{
    for (ComponentSetList::iterator it = this->model->begin();
         it != this->model->end();
         ++it)
    {
        if ((*it).name == name)
        {
            // Update model.
            this->model->erase(it);
            return;
        }
    }
}

void ComponentsController::setComponents(ComponentSetList& model)
{
    this->model = &model;
}
