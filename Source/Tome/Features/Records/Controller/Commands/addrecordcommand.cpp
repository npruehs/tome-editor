#include "addrecordcommand.h"

#include "../recordscontroller.h"

using namespace Tome;

AddRecordCommand::AddRecordCommand(RecordsController& recordsController,
                                   const QString& id,
                                   const QString& displayName,
                                   const QStringList& fieldIds,
                                   const QString& recordSetName)
    : recordsController(recordsController),
      id(id),
      displayName(displayName),
      fieldIds(fieldIds),
      recordSetName(recordSetName)
{
    this->setText(tr("Add Record - %1").arg(id));
}

void AddRecordCommand::undo()
{
    this->recordsController.removeRecord(this->id);
}

void AddRecordCommand::redo()
{
    this->recordsController.addRecord(
                this->id,
                this->displayName,
                this->fieldIds,
                this->recordSetName);
}
