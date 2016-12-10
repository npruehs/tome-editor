#ifndef REMOVECOMPONENTCOMMAND_H
#define REMOVECOMPONENTCOMMAND_H

#include <QUndoCommand>

#include "../../Model/component.h"
#include "../../../Fields/Model/fielddefinitionlist.h"

namespace Tome
{
    class ComponentsController;
    class FieldDefinitionsController;

    class RemoveComponentCommand : public QUndoCommand, public QObject
    {
        public:
            RemoveComponentCommand(ComponentsController& componentsController, FieldDefinitionsController& fieldDefinitionsController, const QString& componentName);

            virtual void undo();
            virtual void redo();

        private:
            ComponentsController& componentsController;
            FieldDefinitionsController& fieldDefinitionsController;

            const QString componentName;

            QString componentSetName;
            FieldDefinitionList affectedFields;
    };
}

#endif // REMOVECOMPONENTCOMMAND_H
