#include "undocontroller.h"

using namespace Tome;


UndoController::UndoController()
    : undoStack(new QUndoStack())
{
}

void UndoController::clear()
{
    this->undoStack->clear();
}

QAction* UndoController::createRedoAction(QObject* parent, const QString& prefix)
{
    return this->undoStack->createRedoAction(parent, prefix);
}

QAction* UndoController::createUndoAction(QObject* parent, const QString& prefix)
{
    return this->undoStack->createUndoAction(parent, prefix);

}

void UndoController::doCommand(QUndoCommand* command)
{
    this->undoStack->push(command);
}
