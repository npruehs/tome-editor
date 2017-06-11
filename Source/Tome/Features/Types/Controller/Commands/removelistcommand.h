#ifndef REMOVELISTCOMMAND_H
#define REMOVELISTCOMMAND_H

#include <QString>
#include <QUndoCommand>

namespace Tome
{
    class TypesController;

    /**
     * @brief Removes a list type from the project.
     */
    class RemoveListCommand : public QUndoCommand, public QObject
    {
        public:
            /**
             * @brief Constructs a new command for removing a list type from the project.
             * @param typesController Controller for adding, updating and removing custom types.
             * @param name Name of the list type to remove.
             */
            RemoveListCommand(TypesController& typesController,
                              const QString& name);

            /**
             * @brief Adds the list type again.
             */
            virtual void undo() Q_DECL_OVERRIDE;

            /**
             * @brief Removes the list type.
             */
            virtual void redo() Q_DECL_OVERRIDE;

     private:
         TypesController& typesController;

         const QString name;

         QString itemType;
         QString customTypeSetName;
    };
}

#endif // REMOVELISTCOMMAND_H
