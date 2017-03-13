#include "revertrecordcommand.h"

#include "../recordscontroller.h"

using namespace Tome;


RevertRecordCommand::RevertRecordCommand(RecordsController& recordsController, const QVariant& recordId)
    : recordsController(recordsController), recordId(recordId)
{
    this->setText(tr("Revert Record - %1").arg(recordId.toString()));
}

void RevertRecordCommand::undo()
{
    qInfo(QString("Undo revert record %1.").arg(this->recordId.toString()).toUtf8().constData());

    // Restore record.
    for (RecordFieldValueMap::iterator it = this->oldRecordFieldValues.begin();
         it != this->oldRecordFieldValues.end();
         ++it)
    {
        this->recordsController.updateRecordFieldValue(this->recordId, it.key(), it.value());
    }
}

void RevertRecordCommand::redo()
{
    // Store current data.
    this->oldRecordFieldValues = this->recordsController.getRecordFieldValues(this->recordId);

    // Revert record.
    this->recordsController.revertRecord(this->recordId);
}
