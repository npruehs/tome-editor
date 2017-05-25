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
            /**
             * @brief Constructs a new controller for performing undo-able commands.
             */
            UndoController();

            /**
             * @brief Removes all recent undo-able commands from the undo stack.
             */
            void clear();

            /**
             * @brief Creates an action for re-doing the most recently undone command.
             * @param parent Parent of the action to create.
             * @param prefix Prefix to show before the name of the command to redo.
             * @return Action for re-doing the most recently undone command.
             */
            QAction* createRedoAction(QObject* parent, const QString& prefix);

            /**
             * @brief Creates an action for un-doing the most recent command.
             * @param parent Parent of the action to create.
             * @param prefix Prefix to show before the name of the command to undo.
             * @return Action for re-doing the most recent command.
             */
            QAction* createUndoAction(QObject* parent, const QString& prefix);

            /**
             * @brief Applies the passed command, pushing it to the undo stack.
             * @param command Command to apply.
             */
            void doCommand(QUndoCommand* command);

            /**
             * @brief Whether any commands have been applied since the project was opened or saved.
             * @return Whether any commands have been applied since the project was opened or saved, or not.
             */
            bool isClean() const;

            /**
             * @brief Marks the undo stack as clean, as if no commands have been applied since the project was opened or saved.
             */
            void setClean();

        signals:
            /**
             * @brief Undo stack has become dirty or clean.
             * @param clean Whether no commands have been applied since the project was opened or saved.
             */
            void undoStackChanged(bool clean);

        private slots:
            void onCleanChanged(bool clean);

        private:
            QUndoStack* undoStack;
    };
}

#endif // UNDOCONTROLLER_H
