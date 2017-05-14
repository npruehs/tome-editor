#ifndef ADDCOMPONENTCOMMAND_H
#define ADDCOMPONENTCOMMAND_H

#include <QUndoCommand>

#include "../../Model/component.h"

namespace Tome
{
    class ComponentsController;

    /**
     * @brief Adds a new component to the project.
     */
    class AddComponentCommand : public QUndoCommand, public QObject
    {
        public:
            /**
             * @brief Constructs a new command for adding a new component to the project.
             * @param componentsController Controller for adding and removing components.
             * @param componentName Name of the component to add.
             * @param componentSetName Name of the component set to add the component to.
             */
            AddComponentCommand(ComponentsController& componentsController,
                                const QString& componentName,
                                const QString& componentSetName);

            /**
             * @brief Removes the component again.
             */
            virtual void undo() Q_DECL_OVERRIDE;

            /**
             * @brief Adds the component.
             */
            virtual void redo() Q_DECL_OVERRIDE;

        private:
            ComponentsController& componentsController;

            const QString componentName;
            const QString componentSetName;

            Component component;
    };
}

#endif // ADDCOMPONENTCOMMAND_H
