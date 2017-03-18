#ifndef ADDMAPCOMMAND_H
#define ADDMAPCOMMAND_H

#include <QString>
#include <QUndoCommand>

namespace Tome
{
    class TypesController;

    class AddMapCommand : public QUndoCommand, public QObject
    {
        public:
            AddMapCommand(TypesController& typesController,
                          const QString& name,
                          const QString& keyType,
                          const QString& valueType,
                          const QString& customTypeSetName);

            virtual void undo() Q_DECL_OVERRIDE;
            virtual void redo() Q_DECL_OVERRIDE;

        private:
            TypesController& typesController;

            const QString name;
            const QString keyType;
            const QString valueType;
            const QString customTypeSetName;
    };
}

#endif // ADDMAPCOMMAND_H
