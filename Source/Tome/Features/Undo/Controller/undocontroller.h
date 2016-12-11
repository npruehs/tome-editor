#ifndef UNDOCONTROLLER_H
#define UNDOCONTROLLER_H

#include <QUndoCommand>
#include <QUndoStack>


namespace Tome
{
    class UndoController : public QObject
    {
            Q_OBJECT

        public:
            UndoController();

            void clear();
            QAction* createRedoAction(QObject* parent, const QString& prefix);
            QAction* createUndoAction(QObject* parent, const QString& prefix);
            void doCommand(QUndoCommand* command);

            int getUndoStackIndex() const;

        signals:
            void undoStackChanged(int index);

        private slots:
            void onIndexChanged(int index);

        private:
            QUndoStack* undoStack;
    };
}

#endif // UNDOCONTROLLER_H
