#ifndef REMOVEDERIVEDTYPECOMMAND_H
#define REMOVEDERIVEDTYPECOMMAND_H

#include <QString>
#include <QUndoCommand>
#include <QVariantMap>

namespace Tome
{
    class TypesController;

    /**
     * @brief Removes a derived type from the project.
     */
    class RemoveDerivedTypeCommand : public QUndoCommand, public QObject
    {
        public:
            /**
             * @brief Constructs a new command for removing a derived type from the project.
             * @param typesController Controller for adding, updating and removing custom types.
             * @param name Name of the derived type to remove.
             */
            RemoveDerivedTypeCommand(TypesController& typesController,
                                     const QString& name);

            /**
             * @brief Adds the derived type again.
             */
            virtual void undo() Q_DECL_OVERRIDE;

            /**
             * @brief Removes the derived type.
             */
            virtual void redo() Q_DECL_OVERRIDE;

        private:
            TypesController& typesController;

            const QString name;

            QString baseType;
            QVariantMap facets;
            QString customTypeSetName;
    };
}

#endif // REMOVEDERIVEDTYPECOMMAND_H
