#ifndef REMOVEMAPCOMMAND_H
#define REMOVEMAPCOMMAND_H

#include <QString>
#include <QUndoCommand>

namespace Tome
{
    class TypesController;

    class RemoveMapCommand : public QUndoCommand
    {
        public:
            RemoveMapCommand(TypesController& typesController,
                             const QString& name);

    virtual void undo();
    virtual void redo();

    private:
        TypesController& typesController;

        const QString name;

        QString keyType;
        QString valueType;
        QString customTypeSetName;
    };
}

#endif // REMOVEMAPCOMMAND_H
