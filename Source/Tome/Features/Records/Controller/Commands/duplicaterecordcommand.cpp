#include "duplicaterecordcommand.h"

#include "../recordscontroller.h"

using namespace Tome;


DuplicateRecordCommand::DuplicateRecordCommand(RecordsController& recordsController,
                                               const QString& existingRecordId,
                                               const QString& newRecordId)
    : recordsController(recordsController),
      existingRecordId(existingRecordId),
      newRecordId(newRecordId)
{
    this->setText(tr("Duplicate Record - %1").arg(existingRecordId));
}

void DuplicateRecordCommand::undo()
{
    qInfo(QString("Undo duplicate record %1.").arg(this->existingRecordId).toUtf8().constData());

    this->recordsController.removeRecord(this->newRecordId);
}

void DuplicateRecordCommand::redo()
{
    this->recordsController.duplicateRecord(this->existingRecordId,
                                            this->newRecordId);
}
