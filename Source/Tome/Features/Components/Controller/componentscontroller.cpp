#include "componentscontroller.h"

#include "../../../Util/listutils.h"
#include "../../../Util/stringutils.h"


using namespace Tome;


ComponentsController::ComponentsController()
{
}

const Component ComponentsController::addComponent(const QString& componentName, const QString& componentSetName)
{
    qInfo(qUtf8Printable(QString("Adding component %1.").arg(componentName)));
    Component component = Component(componentName);

    for (ComponentSetList::iterator it = this->model->begin();
         it != this->model->end();
         ++it)
    {
        ComponentSet& componentSet = *it;

        if (componentSet.name == componentSetName)
        {
            ComponentList& components = componentSet.components;
            int index = findInsertionIndex(components, component, qStringLessThanLowerCase);
            components.insert(index, component);
            emit this->componentAdded(component);
            return component;
        }
    }

    const QString errorMessage = "Component set not found: " + componentSetName;
    qCritical(qUtf8Printable(errorMessage));
    throw std::out_of_range(errorMessage.toStdString());
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

const QStringList ComponentsController::getComponentSetNames() const
{
    QStringList names;

    for (int i = 0; i < this->model->size(); ++i)
    {
        const ComponentSet& componentSet = this->model->at(i);
        names << componentSet.name;
    }

    return names;
}

const QString ComponentsController::getSetNameOfComponent(const Component component) const
{
    for (ComponentSetList::iterator itSets = this->model->begin();
         itSets != this->model->end();
         ++itSets)
    {
        ComponentList& components = (*itSets).components;

        for (ComponentList::iterator it = components.begin();
             it != components.end();
             ++it)
        {
            if (*it == component)
            {
                return (*itSets).name;
            }
        }
    }

    const QString errorMessage = "Component not found: " + component;
    qCritical(qUtf8Printable(errorMessage));
    throw std::out_of_range(errorMessage.toStdString());
}

int ComponentsController::indexOf(const Component& component) const
{
    for (ComponentSetList::iterator it = this->model->begin();
         it != this->model->end();
         ++it)
    {
        const ComponentSet& componentSet = *it;
        int componentIndex = componentSet.components.indexOf(component);

        if (componentIndex >= 0)
        {
            return componentIndex;
        }
    }

    return -1;
}

bool ComponentsController::removeComponent(const Component component)
{
    qInfo(qUtf8Printable(QString("Removing component %1.").arg(component)));

    for (ComponentSetList::iterator itSets = this->model->begin();
         itSets != this->model->end();
         ++itSets)
    {
        ComponentList& components = (*itSets).components;

        for (ComponentList::iterator it = components.begin();
             it != components.end();
             ++it)
        {
            if (*it == component)
            {
                emit this->componentRemoved(component);
                components.erase(it);
                return true;
            }
        }
    }

    return false;
}

bool ComponentsController::removeComponentSet(const QString& name)
{
    for (ComponentSetList::iterator it = this->model->begin();
         it != this->model->end();
         ++it)
    {
        if ((*it).name == name)
        {
            // Update model.
            this->model->erase(it);
            return true;
        }
    }

    return false;
}

void ComponentsController::setComponents(ComponentSetList& model)
{
    this->model = &model;
}
