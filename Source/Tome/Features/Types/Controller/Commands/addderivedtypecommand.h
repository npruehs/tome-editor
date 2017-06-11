#ifndef ADDDERIVEDTYPECOMMAND_H
#define ADDDERIVEDTYPECOMMAND_H

#include <QString>
#include <QUndoCommand>
#include <QVariantMap>

namespace Tome
{
    class TypesController;

    /**
     * @brief Adds a new derived type to the project.
     */
    class AddDerivedTypeCommand : public QUndoCommand, public QObject
    {
        public:
            /**
             * @brief Constructs a new command for adding a new derived type to the project.
             * @param typesController Controller for adding, updating and removing custom types.
             * @param name Name of the type to add.
             * @param baseType Name of the base type of the type to add.
             * @param facets Facet values of the type to add.
             * @param customTypeSetName Name of the type set to add the new type to.
             */
            AddDerivedTypeCommand(TypesController& typesController,
                                  const QString& name,
                                  const QString& baseType,
                                  const QVariantMap& facets,
                                  const QString& customTypeSetName);

            /**
             * @brief Removes the type again.
             */
            virtual void undo() Q_DECL_OVERRIDE;

            /**
             * @brief Adds the type.
             */
            virtual void redo() Q_DECL_OVERRIDE;

        private:
            TypesController& typesController;

            const QString name;
            const QString baseType;
            const QVariantMap facets;
            const QString customTypeSetName;
    };
}

#endif // ADDDERIVEDTYPECOMMAND_H
