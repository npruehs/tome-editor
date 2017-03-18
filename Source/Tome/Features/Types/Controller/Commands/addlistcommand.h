#ifndef ADDLISTCOMMAND_H
#define ADDLISTCOMMAND_H

#include <QString>
#include <QUndoCommand>

namespace Tome
{
    class TypesController;

    class AddListCommand : public QUndoCommand, public QObject
    {
        public:
            AddListCommand(TypesController& typesController,
                           const QString& name,
                           const QString& itemType,
                           const QString& customTypeSetName);

            virtual void undo() Q_DECL_OVERRIDE;
            virtual void redo() Q_DECL_OVERRIDE;

        private:
            TypesController& typesController;

            const QString name;
            const QString itemType;
            const QString customTypeSetName;
    };
}

#endif // ADDLISTCOMMAND_H
