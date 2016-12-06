#ifndef UPDATEDERIVEDTYPECOMMAND_H
#define UPDATEDERIVEDTYPECOMMAND_H

#include <QString>
#include <QUndoCommand>
#include <QVariantMap>

namespace Tome
{
    class TypesController;

    class UpdateDerivedTypeCommand : public QUndoCommand
    {
        public:
            UpdateDerivedTypeCommand(TypesController& typesController,
                                     const QString& oldName,
                                     const QString& newName,
                                     const QString& baseType,
                                     const QVariantMap facets,
                                     const QString& typeSetName);

            virtual void undo();
            virtual void redo();

        private:
            TypesController& typesController;

            const QString oldName;
            const QString newName;
            const QString newBaseType;
            const QVariantMap newFacets;
            const QString newTypeSetName;

            QString oldBaseType;
            QVariantMap oldFacets;
            QString oldTypeSetName;
    };
}

#endif // UPDATEDERIVEDTYPECOMMAND_H
