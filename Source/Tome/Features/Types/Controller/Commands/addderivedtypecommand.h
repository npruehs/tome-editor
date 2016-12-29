#ifndef ADDDERIVEDTYPECOMMAND_H
#define ADDDERIVEDTYPECOMMAND_H

#include <QString>
#include <QUndoCommand>
#include <QVariantMap>

namespace Tome
{
    class TypesController;

    class AddDerivedTypeCommand : public QUndoCommand, public QObject
    {
        public:
            AddDerivedTypeCommand(TypesController& typesController,
                                  const QString& name,
                                  const QString& baseType,
                                  const QVariantMap& facets,
                                  const QString& customTypeSetName);

            virtual void undo();
            virtual void redo();

        private:
            TypesController& typesController;

            const QString name;
            const QString baseType;
            const QVariantMap facets;
            const QString customTypeSetName;
    };
}

#endif // ADDDERIVEDTYPECOMMAND_H
