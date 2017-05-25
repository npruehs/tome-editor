#ifndef ADDENUMERATIONCOMMAND_H
#define ADDENUMERATIONCOMMAND_H

#include <QString>
#include <QStringList>
#include <QUndoCommand>

namespace Tome
{
    class TypesController;

    /**
     * @brief Adds a new enumeration type to the project.
     */
    class AddEnumerationCommand : public QUndoCommand, public QObject
    {
        public:
            /**
             * @brief Constructs a new command for adding a new enumeration type to the project.
             * @param typesController Controller for adding, updating and removing custom types.
             * @param name Name of the enumeration to add.
             * @param enumeration Items of the enumeration to add.
             * @param customTypeSetName Name of the type set to add the new enumeration to.
             */
            AddEnumerationCommand(TypesController& typesController,
                                  const QString& name,
                                  const QStringList& enumeration,
                                  const QString& customTypeSetName);

            /**
             * @brief Removes the enumeration again.
             */
            virtual void undo() Q_DECL_OVERRIDE;

            /**
             * @brief Adds the enumeration.
             */
            virtual void redo() Q_DECL_OVERRIDE;

        private:
            TypesController& typesController;

            const QString name;
            const QStringList enumeration;
            const QString customTypeSetName;
    };
}

#endif // ADDENUMERATIONCOMMAND_H
