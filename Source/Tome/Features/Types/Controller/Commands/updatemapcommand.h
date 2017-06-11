#ifndef UPDATEMAPCOMMAND_H
#define UPDATEMAPCOMMAND_H

#include <QUndoCommand>

namespace Tome
{
    class TypesController;

    /**
     * @brief Updates the properties of a map type.
     */
    class UpdateMapCommand : public QUndoCommand, public QObject
    {
        public:
            /**
             * @brief Constructs a new command for updating the properties of a map type.
             * @param typesController Controller for adding, updating and removing custom types.
             * @param oldName Current name of the map type to update.
             * @param newName New name of the map type to update.
             * @param newKeyType New type of the keys of the map type to update.
             * @param newValueType New type of the values of the map type to update.
             * @param newTypeSetName Name of the new type set the map type should belong to.
             */
            UpdateMapCommand(TypesController& typesController,
                             const QString& oldName,
                             const QString& newName,
                             const QString& newKeyType,
                             const QString& newValueType,
                             const QString& newTypeSetName);

            /**
             * @brief Reverts all property changes made by this command.
             */
            virtual void undo() Q_DECL_OVERRIDE;

            /**
             * @brief Updates the properties of the list type.
             */
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
