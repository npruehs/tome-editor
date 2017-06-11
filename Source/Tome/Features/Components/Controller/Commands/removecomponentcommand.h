#ifndef REMOVECOMPONENTCOMMAND_H
#define REMOVECOMPONENTCOMMAND_H

#include <QUndoCommand>

#include "../../Model/component.h"
#include "../../../Fields/Model/fielddefinitionlist.h"

namespace Tome
{
    class ComponentsController;
    class FieldDefinitionsController;

    /**
     * @brief Removes a component from the project.
     */
    class RemoveComponentCommand : public QUndoCommand, public QObject
    {
        public:
            /**
             * @brief RemoveComponentCommand Constructs a new command for removing a component from the project.
             * @param componentsController Controller for adding and removing components.
             * @param fieldDefinitionsController Controller for adding, updating and removing field definitions.
             * @param componentName Name of the component to remove.
             */
            RemoveComponentCommand(ComponentsController& componentsController,
                                   FieldDefinitionsController& fieldDefinitionsController,
                                   const QString& componentName);

            /**
             * @brief Adds the component again.
             */
            virtual void undo() Q_DECL_OVERRIDE;

            /**
             * @brief Removes the component.
             */
            virtual void redo() Q_DECL_OVERRIDE;

        private:
            ComponentsController& componentsController;
            FieldDefinitionsController& fieldDefinitionsController;

            const QString componentName;

            QString componentSetName;
            FieldDefinitionList affectedFields;
    };
}

#endif // REMOVECOMPONENTCOMMAND_H
