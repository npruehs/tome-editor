#ifndef ADDMAPCOMMAND_H
#define ADDMAPCOMMAND_H

#include <QString>
#include <QUndoCommand>

namespace Tome
{
    class TypesController;

    /**
     * @brief Adds a new map type to the project.
     */
    class AddMapCommand : public QUndoCommand, public QObject
    {
        public:
            /**
             * @brief Constructs a new command for adding a new map type to the project.
             * @param typesController Controller for adding, updating and removing custom types.
             * @param name Name of the map type to add.
             * @param keyType Type of the keys of the map type to add.
             * @param valueType Type of the values of the map type to add.
             * @param customTypeSetName Name of the type set to add the new map type to.
             */
            AddMapCommand(TypesController& typesController,
                          const QString& name,
                          const QString& keyType,
                          const QString& valueType,
                          const QString& customTypeSetName);

            /**
             * @brief Removes the map type again.
             */
            virtual void undo() Q_DECL_OVERRIDE;

            /**
             * @brief Adds the map type.
             */
            virtual void redo() Q_DECL_OVERRIDE;

        private:
            TypesController& typesController;

            const QString name;
            const QString keyType;
            const QString valueType;
            const QString customTypeSetName;
    };
}

#endif // ADDMAPCOMMAND_H
