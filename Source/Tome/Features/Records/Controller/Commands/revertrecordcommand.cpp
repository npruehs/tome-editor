#include "revertrecordcommand.h"

#include "../recordscontroller.h"

using namespace Tome;


RevertRecordCommand::RevertRecordCommand(RecordsController& recordsController, const QString& recordId)
    : recordsController(recordsController), recordId(recordId)
{
    this->setText(tr("Revert Record - %1").arg(recordId));
}

void RevertRecordCommand::undo()
{
    qInfo(QString("Undo revert record %1.").arg(this->recordId).toUtf8().constData());

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
