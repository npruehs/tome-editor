#ifndef COMPONENTSCONTROLLER_H
#define COMPONENTSCONTROLLER_H

#include "../Model/component.h"
#include "../Model/componentlist.h"
#include "../Model/componentsetlist.h"

namespace Tome
{
    class ComponentsController
    {
        public:
            ComponentsController();

            const Component addComponent(const QString& componentName);
            void addComponentSet(const ComponentSet& componentSet);
            const ComponentList getComponents() const;
            const ComponentSetList& getComponentSets() const;
            int indexOf(const Component& component) const;
            void removeComponent(const Component component);
            void removeComponentSet(const QString& name);
            void setComponents(ComponentSetList& model);

        private:
            ComponentSetList* model;
    };
}

#endif // COMPONENTSCONTROLLER_H
