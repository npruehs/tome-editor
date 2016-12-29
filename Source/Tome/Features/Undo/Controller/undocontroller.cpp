#include "undocontroller.h"

using namespace Tome;


UndoController::UndoController()
    : undoStack(new QUndoStack())
{
    connect(this->undoStack,
            SIGNAL(cleanChanged(bool)),
            SLOT(onCleanChanged(bool)));
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

bool UndoController::isClean() const
{
    return this->undoStack->isClean();
}

void UndoController::setClean()
{
    this->undoStack->setClean();
}

void UndoController::onCleanChanged(bool clean)
{
    // Notify listeners.
    emit this->undoStackChanged(clean);
}
