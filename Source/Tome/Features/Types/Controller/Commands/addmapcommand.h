#ifndef ADDMAPCOMMAND_H
#define ADDMAPCOMMAND_H

#include <QString>
#include <QUndoCommand>

namespace Tome
{
    class TypesController;

    class AddMapCommand : public QUndoCommand
    {
        public:
            AddMapCommand(TypesController& typesController,
                          const QString& name,
                          const QString& keyType,
                          const QString& valueType,
                          const QString& customTypeSetName);

            virtual void undo();
            virtual void redo();

        private:
            TypesController& typesController;

            const QString name;
            const QString keyType;
            const QString valueType;
            const QString customTypeSetName;
    };
}

#endif // ADDMAPCOMMAND_H
