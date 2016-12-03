#ifndef COMPONENTSCONTROLLER_H
#define COMPONENTSCONTROLLER_H

#include <QObject>

#include "../Model/component.h"
#include "../Model/componentlist.h"
#include "../Model/componentsetlist.h"

namespace Tome
{
    class ComponentsController : public QObject
    {
            Q_OBJECT

        public:
            ComponentsController();

            const Component addComponent(const QString& componentName, const QString& componentSetName);
            void addComponentSet(const ComponentSet& componentSet);
            const ComponentList getComponents() const;
            const ComponentSetList& getComponentSets() const;
            const QStringList getComponentSetNames() const;
            int indexOf(const Component& component) const;
            void removeComponent(const Component component);
            void removeComponentSet(const QString& name);
            void setComponents(ComponentSetList& model);

        signals:
            void componentAdded(const Tome::Component& component);
            void componentRemoved(const Tome::Component& component);

        private:
            ComponentSetList* model;
    };
}

#endif // COMPONENTSCONTROLLER_H
