#include "updaterecordcommand.h"

#include "../recordscontroller.h"

using namespace Tome;


UpdateRecordCommand::UpdateRecordCommand(RecordsController& recordsController,
                                         const QVariant& oldId,
                                         const QVariant& newId,
                                         const QString& newDisplayName,
                                         const QString& newEditorIconFieldId,
                                         const QStringList& newFieldIds,
                                         const QString& newRecordSetName)
    : recordsController(recordsController),
      oldId(oldId),
      newId(newId),
      newDisplayName(newDisplayName),
      newEditorIconFieldId(newEditorIconFieldId),
      newFieldIds(newFieldIds),
      newRecordSetName(newRecordSetName)
{
    this->setText(tr("Update Record - %1").arg(oldDisplayName));
}

void UpdateRecordCommand::undo()
{
    qInfo(qUtf8Printable(QString("Undo update record %1.").arg(this->oldDisplayName)));

    // Restore record.
    this->recordsController.updateRecord(this->newId,
                                         this->oldId,
                                         this->oldDisplayName,
                                         this->oldEditorIconFieldId,
                                         this->oldFieldIds,
                                         this->oldRecordSetName);
}

void UpdateRecordCommand::redo()
{
    // Store current data.
    const Record& record = this->recordsController.getRecord(this->oldId);

    this->oldDisplayName = record.displayName;
    this->oldEditorIconFieldId = record.editorIconFieldId;
    this->oldRecordSetName = record.recordSetName;
    this->oldFieldIds = QStringList();

    for (RecordFieldValueMap::const_iterator it = record.fieldValues.cbegin();
         it != record.fieldValues.cend();
         ++it)
    {
        this->oldFieldIds << it.key();
    }

    // Update record.
    this->recordsController.updateRecord(this->oldId,
                                         this->newId,
                                         this->newDisplayName,
                                         this->newEditorIconFieldId,
                                         this->newFieldIds,
                                         this->newRecordSetName);
}
