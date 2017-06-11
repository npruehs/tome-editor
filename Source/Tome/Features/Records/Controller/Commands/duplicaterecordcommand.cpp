#include "duplicaterecordcommand.h"

#include "../recordscontroller.h"

using namespace Tome;


DuplicateRecordCommand::DuplicateRecordCommand(RecordsController& recordsController,
                                               const QVariant& existingRecordId,
                                               const QVariant& newRecordId)
    : recordsController(recordsController),
      existingRecordId(existingRecordId),
      newRecordId(newRecordId)
{
    this->setText(tr("Duplicate Record - %1").arg(existingRecordId.toString()));
}

void DuplicateRecordCommand::undo()
{
    qInfo(qUtf8Printable(QString("Undo duplicate record %1.").arg(this->existingRecordId.toString())));

    this->recordsController.removeRecord(this->newRecordId);
}

void DuplicateRecordCommand::redo()
{
    this->recordsController.duplicateRecord(this->existingRecordId,
                                            this->newRecordId);
}
