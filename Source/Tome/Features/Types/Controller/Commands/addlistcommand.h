#ifndef ADDLISTCOMMAND_H
#define ADDLISTCOMMAND_H

#include <QString>
#include <QUndoCommand>

namespace Tome
{
    class TypesController;

    class AddListCommand : public QUndoCommand
    {
        public:
            AddListCommand(TypesController& typesController,
                           const QString& name,
                           const QString& itemType,
                           const QString& customTypeSetName);

            virtual void undo();
            virtual void redo();

        private:
            TypesController& typesController;

            const QString name;
            const QString itemType;
            const QString customTypeSetName;
    };
}

#endif // ADDLISTCOMMAND_H
