#ifndef ADDCOMPONENTCOMMAND_H
#define ADDCOMPONENTCOMMAND_H

#include <QUndoCommand>

#include "../../Model/component.h"

namespace Tome
{
    class ComponentsController;

    class AddComponentCommand : public QUndoCommand
    {
        public:
            AddComponentCommand(ComponentsController& componentsController, const QString& componentName, const QString& componentSetName);

            virtual void undo();
            virtual void redo();

            Component component;

        private:
            ComponentsController& componentsController;

            const QString componentName;
            const QString componentSetName;


    };
}

#endif // ADDCOMPONENTCOMMAND_H
