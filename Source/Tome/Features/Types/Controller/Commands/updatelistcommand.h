#ifndef UPDATELISTCOMMAND_H
#define UPDATELISTCOMMAND_H

#include <QString>
#include <QUndoCommand>

namespace Tome
{
    class TypesController;

    class UpdateListCommand : public QUndoCommand
    {
        public:
            UpdateListCommand(TypesController& typesController,
                              const QString& oldName,
                              const QString& newName,
                              const QString& newItemType,
                              const QString& newTypeSetName);

            virtual void undo();
            virtual void redo();

        private:
            TypesController& typesController;

            const QString oldName;
            const QString newName;
            const QString newItemType;
            const QString newTypeSetName;

            QString oldItemType;
            QString oldTypeSetName;
    };
}

#endif // UPDATELISTCOMMAND_H
