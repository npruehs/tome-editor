#ifndef COMPONENTSCONTROLLER_H
#define COMPONENTSCONTROLLER_H

#include <QObject>

#include "../Model/component.h"
#include "../Model/componentlist.h"
#include "../Model/componentsetlist.h"

namespace Tome
{
    /**
     * @brief Controller for adding and removing components.
     */
    class ComponentsController : public QObject
    {
            Q_OBJECT

        public:
            /**
             * @brief Constructs a new controller for adding and removing components.
             */
            ComponentsController();

            /**
             * @brief Adds a new component to the specified component set.
             * @param componentName Name of the component to add.
             * @param componentSetName Name of the component set to add the component to.
             * @return
             */
            const Component addComponent(const QString& componentName,
                                         const QString& componentSetName);

            /**
             * @brief Adds a new component set to the project.
             * @param componentSet Name of the component set to add.
             */
            void addComponentSet(const ComponentSet& componentSet);

            /**
             * @brief Gets a list of all components in the project.
             * @return List of all components in the project.
             */
            const ComponentList getComponents() const;

            /**
             * @brief Gets a list of all component sets in the project.
             * @return List of all component sets in the project.
             */
            const ComponentSetList& getComponentSets() const;

            /**
             * @brief Gets a list of the names of all component sets in the project.
             * @return List of the names of all component sets in the project.
             */
            const QStringList getComponentSetNames() const;

            /**
             * @brief Gets the name of the component set containing the specified component.
             *
             * Throws std::out_of_range if that component could not be found.
             *
             * @param component Component to get the name of the containing set of.
             * @return Name of the set containing the specified component.
             */
            const QString getSetNameOfComponent(const Component component) const;

            /**
             * @brief Gets the index of the specified component within its set.
             *
             * Returns -1 if the component could not be found.
             *
             * @param component Component to get the index of.
             * @return Index of the specified component within its set, or -1 if that component could not be found.
             */
            int indexOf(const Component& component) const;

            /**
             * @brief Removes the specified component from the project.
             * @param component Component to remove.
             * @return true, if the component was removed, and false if the component could not be found.
             */
            bool removeComponent(const Component component);

            /**
             * @brief Removes the component set with the specified name from the project, including all components.
             * @param name Name of the component set to remove.
             * @return true, if the component set was removed, and false if the component set could not be found.
             */
            bool removeComponentSet(const QString& name);

            /**
             * @brief Uses the specified list of component sets as model for this controller.
             * @param model New list of component sets to use as model for this controller.
             */
            void setComponents(ComponentSetList& model);

        signals:
            /**
             * @brief A new component has been added to the project.
             * @param component Component that has been added to the project.
             */
            void componentAdded(const Tome::Component& component);

            /**
             * @brief A component has been removed from the project.
             * @param component Component that has been removed from the project.
             */
            void componentRemoved(const Tome::Component& component);

        private:
            ComponentSetList* model;
    };
}

#endif // COMPONENTSCONTROLLER_H
