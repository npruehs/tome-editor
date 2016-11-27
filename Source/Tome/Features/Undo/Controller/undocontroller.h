#ifndef UNDOCONTROLLER_H
#define UNDOCONTROLLER_H

#include <QUndoCommand>
#include <QUndoStack>


namespace Tome
{
    class UndoController
    {
        public:
            UndoController();

            QAction* createRedoAction(QObject* parent, const QString& prefix);
            QAction* createUndoAction(QObject* parent, const QString& prefix);
            void doCommand(QUndoCommand* command);

        private:
            QUndoStack* undoStack;
    };
}

#endif // UNDOCONTROLLER_H
