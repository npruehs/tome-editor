#ifndef UNDOCONTROLLER_H
#define UNDOCONTROLLER_H

#include <QUndoCommand>
#include <QUndoStack>


namespace Tome
{
    /**
     * @brief Controller for performing undo-able commands.
     */
    class UndoController : public QObject
    {
            Q_OBJECT

        public:
            UndoController();

            void clear();
            QAction* createRedoAction(QObject* parent, const QString& prefix);
            QAction* createUndoAction(QObject* parent, const QString& prefix);
            void doCommand(QUndoCommand* command);
            bool isClean() const;
            void setClean();

        signals:
            void undoStackChanged(bool clean);

        private slots:
            void onCleanChanged(bool clean);

        private:
            QUndoStack* undoStack;
    };
}

#endif // UNDOCONTROLLER_H
