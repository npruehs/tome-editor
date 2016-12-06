#ifndef REMOVEDERIVEDTYPECOMMAND_H
#define REMOVEDERIVEDTYPECOMMAND_H

#include <QString>
#include <QUndoCommand>
#include <QVariantMap>

namespace Tome
{
    class TypesController;

    class RemoveDerivedTypeCommand : public QUndoCommand
    {
        public:
            RemoveDerivedTypeCommand(TypesController& typesController,
                                     const QString& name);

            virtual void undo();
            virtual void redo();

        private:
            TypesController& typesController;

            const QString name;

            QString baseType;
            QVariantMap facets;
            QString customTypeSetName;
    };
}

#endif // REMOVEDERIVEDTYPECOMMAND_H
