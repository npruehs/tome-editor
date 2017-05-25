#ifndef ADDLISTCOMMAND_H
#define ADDLISTCOMMAND_H

#include <QString>
#include <QUndoCommand>

namespace Tome
{
    class TypesController;

    /**
     * @brief Adds a new list type to the project.
     */
    class AddListCommand : public QUndoCommand, public QObject
    {
        public:
            /**
             * @brief Constructs a new command for adding a new list type to the project.
             * @param typesController Controller for adding, updating and removing custom types.
             * @param name Name of the list type to add.
             * @param itemType Type of the items of the list type to add.
             * @param customTypeSetName Name of the type set to add the new list type to.
             */
            AddListCommand(TypesController& typesController,
                           const QString& name,
                           const QString& itemType,
                           const QString& customTypeSetName);

            /**
             * @brief Removes the list type again.
             */
            virtual void undo() Q_DECL_OVERRIDE;

            /**
             * @brief Adds the list type.
             */
            virtual void redo() Q_DECL_OVERRIDE;

        private:
            TypesController& typesController;

            const QString name;
            const QString itemType;
            const QString customTypeSetName;
    };
}

#endif // ADDLISTCOMMAND_H
