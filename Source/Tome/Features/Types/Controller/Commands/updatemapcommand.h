#ifndef UPDATEMAPCOMMAND_H
#define UPDATEMAPCOMMAND_H

#include <QUndoCommand>

namespace Tome
{
    class TypesController;

    class UpdateMapCommand : public QUndoCommand, public QObject
    {
        public:
            UpdateMapCommand(TypesController& typesController,
                             const QString& oldName,
                             const QString& newName,
                             const QString& newKeyType,
                             const QString& newValueType,
                             const QString& newTypeSetName);

            virtual void undo() Q_DECL_OVERRIDE;
            virtual void redo() Q_DECL_OVERRIDE;

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
