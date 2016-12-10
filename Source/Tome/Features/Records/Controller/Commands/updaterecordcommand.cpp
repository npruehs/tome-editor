#include "updaterecordcommand.h"

#include "../recordscontroller.h"

using namespace Tome;


UpdateRecordCommand::UpdateRecordCommand(RecordsController& recordsController,
                                         const QString& oldId,
                                         const QString& newId,
                                         const QString& newDisplayName,
                                         const QStringList& newFieldIds,
                                         const QString& newRecordSetName)
    : recordsController(recordsController),
      oldId(oldId),
      newId(newId),
      newDisplayName(newDisplayName),
      newFieldIds(newFieldIds),
      newRecordSetName(newRecordSetName)
{
    this->setText(tr("Update Record - %1").arg(oldId));
}

void UpdateRecordCommand::undo()
{
    // Restore record.
    this->recordsController.updateRecord(this->newId,
                                         this->oldId,
                                         this->oldDisplayName,
                                         this->oldFieldIds,
                                         this->oldRecordSetName);
}

void UpdateRecordCommand::redo()
{
    // Store current data.
    const Record& record = this->recordsController.getRecord(this->oldId);

    this->oldDisplayName = record.displayName;
    this->oldRecordSetName = record.recordSetName;
    this->oldFieldIds = QStringList();

    for (RecordFieldValueMap::const_iterator it = record.fieldValues.begin();
         it != record.fieldValues.end();
         ++it)
    {
        this->oldFieldIds << it.key();
    }

    // Update record.
    this->recordsController.updateRecord(this->oldId,
                                         this->newId,
                                         this->newDisplayName,
                                         this->newFieldIds,
                                         this->newRecordSetName);
}
