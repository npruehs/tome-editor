#ifndef REMOVEDERIVEDTYPECOMMAND_H
#define REMOVEDERIVEDTYPECOMMAND_H

#include <QString>
#include <QUndoCommand>
#include <QVariantMap>

namespace Tome
{
    class TypesController;

    class RemoveDerivedTypeCommand : public QUndoCommand, public QObject
    {
        public:
            RemoveDerivedTypeCommand(TypesController& typesController,
                                     const QString& name);

            virtual void undo() Q_DECL_OVERRIDE;
            virtual void redo() Q_DECL_OVERRIDE;

        private:
            TypesController& typesController;

            const QString name;

            QString baseType;
            QVariantMap facets;
            QString customTypeSetName;
    };
}

#endif // REMOVEDERIVEDTYPECOMMAND_H
