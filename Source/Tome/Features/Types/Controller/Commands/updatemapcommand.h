#ifndef UPDATEMAPCOMMAND_H
#define UPDATEMAPCOMMAND_H

#include <QUndoCommand>

namespace Tome
{
    class TypesController;

    class UpdateMapCommand : public QUndoCommand
    {
        public:
            UpdateMapCommand(TypesController& typesController,
                             const QString& oldName,
                             const QString& newName,
                             const QString& newKeyType,
                             const QString& newValueType,
                             const QString& newTypeSetName);

            virtual void undo();
            virtual void redo();

        private:
            TypesController& typesController;

            const QString oldName;
            const QString newName;
            const QString newKeyType;
            const QString newValueType;
            const QString newTypeSetName;

            QString oldKeyType;
            QString oldValueType;
            QString oldTypeSetName;
    };
}

#endif // UPDATEMAPCOMMAND_H
