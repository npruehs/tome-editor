#ifndef UPDATEDERIVEDTYPECOMMAND_H
#define UPDATEDERIVEDTYPECOMMAND_H

#include <QString>
#include <QUndoCommand>
#include <QVariantMap>

namespace Tome
{
    class TypesController;

    /**
     * @brief Updates the properties of a derived type.
     */
    class UpdateDerivedTypeCommand : public QUndoCommand, public QObject
    {
        public:
            /**
             * @brief Constructs a new command for updating the properties of a derived type.
             * @param typesController Controller for adding, updating and removing custom types.
             * @param oldName Current name of the derived type to update.
             * @param newName New name of the derived type to update.
             * @param baseType Name of the new base type of the type to update.
             * @param facets New facet values of the type to update.
             * @param typeSetName Name of the new type set the derived type should belong to.
             */
            UpdateDerivedTypeCommand(TypesController& typesController,
                                     const QString& oldName,
                                     const QString& newName,
                                     const QString& baseType,
                                     const QVariantMap facets,
                                     const QString& typeSetName);

            /**
             * @brief Reverts all property changes made by this command.
             */
            virtual void undo() Q_DECL_OVERRIDE;

            /**
             * @brief Updates the properties of the derived type.
             */
            virtual void redo() Q_DECL_OVERRIDE;

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
