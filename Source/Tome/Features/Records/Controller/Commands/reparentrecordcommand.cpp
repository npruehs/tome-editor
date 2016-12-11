#include "reparentrecordcommand.h"

#include "../recordscontroller.h"

using namespace Tome;


ReparentRecordCommand::ReparentRecordCommand(RecordsController& recordsController,
                                             const QString& recordId,
                                             const QString& newParentId)
    : recordsController(recordsController),
      recordId(recordId),
      newParentId(newParentId)
{
    this->setText(tr("Reparent Record - %1").arg(recordId));
}

void ReparentRecordCommand::undo()
{
    qInfo(QString("Undo reparent record %1.").arg(this->recordId).toUtf8().constData());

    // Restore parent.
    this->recordsController.reparentRecord(this->recordId, this->oldParentId);
}

void ReparentRecordCommand::redo()
{
    // Store current parent.
    const Record& record = this->recordsController.getRecord(this->recordId);
    this->oldParentId = record.parentId;

    // Reparent record.
    this->recordsController.reparentRecord(this->recordId, this->newParentId);
}