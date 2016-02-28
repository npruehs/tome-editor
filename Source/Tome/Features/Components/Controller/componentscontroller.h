#ifndef COMPONENTSCONTROLLER_H
#define COMPONENTSCONTROLLER_H

#include "../Model/component.h"
#include "../Model/componentlist.h"

namespace Tome
{
    class ComponentsController
    {
        public:
            ComponentsController();

            const Component addComponent(const QString& componentName);
            const ComponentList& getComponents() const;
            int indexOf(const Component& component) const;
            void removeComponent(const Component component);
            void setComponents(ComponentList& model);

        private:
            ComponentList* model;
    };
}

#endif // COMPONENTSCONTROLLER_H
