#ifndef UPDATEENUMERATIONCOMMAND_H
#define UPDATEENUMERATIONCOMMAND_H

#include <QString>
#include <QStringList>
#include <QUndoCommand>

namespace Tome
{
    class TypesController;

    class UpdateEnumerationCommand : public QUndoCommand, public QObject
    {
        public:
            UpdateEnumerationCommand(TypesController& typesController,
                                     const QString& oldName,
                                     const QString& newName,
                                     const QStringList& newEnumeration,
                                     const QString& newTypeSetName);

            virtual void undo() Q_DECL_OVERRIDE;
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
