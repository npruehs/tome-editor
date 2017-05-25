#include "updaterecordfieldvaluecommand.h"

#include "../recordscontroller.h"

using namespace Tome;


UpdateRecordFieldValueCommand::UpdateRecordFieldValueCommand(RecordsController& recordsController,
                                                             const QVariant& recordId,
                                                             const QString& fieldId,
                                                             const QVariant& newFieldValue)
    : recordsController(recordsController),
      recordId(recordId),
      fieldId(fieldId),
      newFieldValue(newFieldValue)
{
    this->setText(tr("Set Field Value - %1").arg(fieldId));
}

void UpdateRecordFieldValueCommand::undo()
{
    qInfo(qUtf8Printable(QString("Undo set field value %1.").arg(this->fieldId)));

    // Restore field value.
    this->recordsController.updateRecordFieldValue(this->recordId, this->fieldId, this->oldFieldValue);
}

void UpdateRecordFieldValueCommand::redo()
{
    // Store current data.
    const RecordFieldValueMap fieldValues = this->recordsController.getRecordFieldValues(this->recordId);
    this->oldFieldValue = fieldValues[fieldId];

    // Update field value.
    this->recordsController.updateRecordFieldValue(this->recordId, this->fieldId, this->newFieldValue);
}
