#ifndef REMOVEENUMERATIONCOMMAND_H
#define REMOVEENUMERATIONCOMMAND_H

#include <QString>
#include <QStringList>
#include <QUndoCommand>

namespace Tome
{
    class TypesController;

    /**
     * @brief Removes an enumeration from the project.
     */
    class RemoveEnumerationCommand : public QUndoCommand, public QObject
    {
        public:
            /**
             * @brief Constructs a new command for removing an enumeration from the project.
             * @param typesController Controller for adding, updating and removing custom types.
             * @param name Name of the enumeration to remove.
             */
            RemoveEnumerationCommand(TypesController& typesController,
                                     const QString& name);

            /**
             * @brief Adds the enumeration again.
             */
            virtual void undo() Q_DECL_OVERRIDE;

            /**
             * @brief Removes the enumeration.
             */
            virtual void redo() Q_DECL_OVERRIDE;

        private:
            TypesController& typesController;

            const QString name;

            QStringList enumeration;
            QString customTypeSetName;
    };
}

#endif // REMOVEENUMERATIONCOMMAND_H
