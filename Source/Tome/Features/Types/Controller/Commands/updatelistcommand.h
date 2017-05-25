#ifndef UPDATELISTCOMMAND_H
#define UPDATELISTCOMMAND_H

#include <QString>
#include <QUndoCommand>

namespace Tome
{
    class TypesController;

    /**
     * @brief Updates the properties of a list type.
     */
    class UpdateListCommand : public QUndoCommand, public QObject
    {
        public:
            /**
             * @brief Constructs a new command for updating the properties of a list type.
             * @param typesController Controller for adding, updating and removing custom types.
             * @param oldName Current name of the list type to update.
             * @param newName New name of the list type to update.
             * @param newItemType New type of the items of the list type to update.
             * @param newTypeSetName Name of the new type set the list type should belong to.
             */
            UpdateListCommand(TypesController& typesController,
                              const QString& oldName,
                              const QString& newName,
                              const QString& newItemType,
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
            const QString newItemType;
            const QString newTypeSetName;

            QString oldItemType;
            QString oldTypeSetName;
    };
}

#endif // UPDATELISTCOMMAND_H
