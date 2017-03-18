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

            virtual void undo() Q_DECL_OVERRIDE;
            virtual void redo() Q_DECL_OVERRIDE;

        private:
            TypesController& typesController;

            const QString name;
            const QStringList enumeration;
            const QString customTypeSetName;
    };
}

#endif // ADDENUMERATIONCOMMAND_H
