#ifndef UPDATEENUMERATIONCOMMAND_H
#define UPDATEENUMERATIONCOMMAND_H

#include <QString>
#include <QStringList>
#include <QUndoCommand>

namespace Tome
{
    class TypesController;

    /**
     * @brief Updates the properties of an enumeration.
     */
    class UpdateEnumerationCommand : public QUndoCommand, public QObject
    {
        public:
            /**
             * @brief Constructs a new command for updating the properties of an enumeration.
             * @param typesController Controller for adding, updating and removing custom types.
             * @param oldName Current name of the enumeration to update.
             * @param newName New name of the enumeration to update.
             * @param newEnumeration New items of the enumeration to update.
             * @param newTypeSetName Name of the new type set the enumeration should belong to.
             */
            UpdateEnumerationCommand(TypesController& typesController,
                                     const QString& oldName,
                                     const QString& newName,
                                     const QStringList& newEnumeration,
                                     const QString& newTypeSetName);

            /**
             * @brief Reverts all property changes made by this command.
             */
            virtual void undo() Q_DECL_OVERRIDE;

            /**
             * @brief Updates the properties of the enumeration.
             */
            virtual void redo() Q_DECL_OVERRIDE;

        private:
            TypesController& typesController;

            const QString oldName;
            const QString newName;
            const QStringList newEnumeration;
            const QString newTypeSetName;

            QStringList oldEnumeration;
            QString oldTypeSetName;
    };
}

#endif // UPDATEENUMERATIONCOMMAND_H
