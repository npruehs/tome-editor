#ifndef REMOVELISTCOMMAND_H
#define REMOVELISTCOMMAND_H

#include <QString>
#include <QUndoCommand>

namespace Tome
{
    class TypesController;

    class RemoveListCommand : public QUndoCommand, public QObject
    {
        public:
            RemoveListCommand(TypesController& typesController,
                              const QString& name);

     virtual void undo();
     virtual void redo();

     private:
         TypesController& typesController;

         const QString name;

         QString itemType;
         QString customTypeSetName;
    };
}

#endif // REMOVELISTCOMMAND_H
