#ifndef ADDENUMERATIONCOMMAND_H
#define ADDENUMERATIONCOMMAND_H

#include <QString>
#include <QStringList>
#include <QUndoCommand>

namespace Tome
{
    class TypesController;

    class AddEnumerationCommand : public QUndoCommand, public QObject
    {
        public:
            AddEnumerationCommand(TypesController& typesController,
                                  const QString& name,
                                  const QStringList& enumeration,
                                  const QString& customTypeSetName);

            virtual void undo();
            virtual void redo();

        private:
            TypesController& typesController;

            const QString name;
            const QStringList enumeration;
            const QString customTypeSetName;
    };
}

#endif // ADDENUMERATIONCOMMAND_H
