#ifndef UPDATELISTCOMMAND_H
#define UPDATELISTCOMMAND_H

#include <QString>
#include <QUndoCommand>

namespace Tome
{
    class TypesController;

    class UpdateListCommand : public QUndoCommand, public QObject
    {
        public:
            UpdateListCommand(TypesController& typesController,
                              const QString& oldName,
                              const QString& newName,
                              const QString& newItemType,
                              const QString& newTypeSetName);

            virtual void undo() Q_DECL_OVERRIDE;
            virtual void redo() Q_DECL_OVERRIDE;

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
