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
    this->setText("Duplicate Record - " + existingRecordId);
}

void DuplicateRecordCommand::undo()
{
    this->recordsController.removeRecord(this->newRecordId);
}

void DuplicateRecordCommand::redo()
{
    this->recordsController.duplicateRecord(this->existingRecordId,
                                            this->newRecordId);
}
