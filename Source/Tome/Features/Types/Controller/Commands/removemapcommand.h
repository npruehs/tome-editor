#ifndef REMOVEMAPCOMMAND_H
#define REMOVEMAPCOMMAND_H

#include <QString>
#include <QUndoCommand>

namespace Tome
{
    class TypesController;

    /**
     * @brief Removes a map type from the project.
     */
    class RemoveMapCommand : public QUndoCommand, public QObject
    {
        public:
            /**
             * @brief Constructs a new command for removing a map type from the project.
             * @param typesController Controller for adding, updating and removing custom types.
             * @param name Name of the map type to remove.
             */
            RemoveMapCommand(TypesController& typesController,
                             const QString& name);

            /**
             * @brief Adds the map type again.
             */
            virtual void undo() Q_DECL_OVERRIDE;

            /**
             * @brief Removes the map type.
             */
            virtual void redo() Q_DECL_OVERRIDE;

    private:
        TypesController& typesController;

        const QString name;

        QString keyType;
        QString valueType;
        QString customTypeSetName;
    };
}

#endif // REMOVEMAPCOMMAND_H
