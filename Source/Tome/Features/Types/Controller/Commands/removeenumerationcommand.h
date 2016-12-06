#ifndef REMOVEENUMERATIONCOMMAND_H
#define REMOVEENUMERATIONCOMMAND_H

#include <QString>
#include <QStringList>
#include <QUndoCommand>

namespace Tome
{
    class TypesController;

    class RemoveEnumerationCommand : public QUndoCommand
    {
        public:
            RemoveEnumerationCommand(TypesController& typesController,
                                     const QString& name);

            virtual void undo();
            virtual void redo();

        private:
            TypesController& typesController;

            const QString name;

            QStringList enumeration;
            QString customTypeSetName;
    };
}

#endif // REMOVEENUMERATIONCOMMAND_H
