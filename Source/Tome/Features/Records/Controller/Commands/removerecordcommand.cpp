#include "removerecordcommand.h"

#include "../recordscontroller.h"
#include "../../../Fields/Controller/fielddefinitionscontroller.h"
#include "../../../Types/Controller/typescontroller.h"
#include "../../../Types/Model/builtintype.h"

using namespace Tome;


RemoveRecordCommand::RemoveRecordCommand(RecordsController& recordsController,
                                         FieldDefinitionsController& fieldDefinitionsController,
                                         TypesController& typesController,
                                         const QVariant& id)
    : recordsController(recordsController),
      fieldDefinitionsController(fieldDefinitionsController),
      typesController(typesController),
      id(id)
{
    this->setText(tr("Remove Record - %1").arg(id.toString()));
}

void RemoveRecordCommand::undo()
{
    qInfo(qUtf8Printable(QString("Undo remove record %1.").arg(this->id.toString())));

    // Add records again.
    for (RecordList::iterator itRecords = this->removedRecords.begin();
         itRecords != this->removedRecords.end();
         ++itRecords)
    {
        const Record record = *itRecords;

        this->recordsController.addRecord(record.id,
                                          record.displayName,
                                          record.editorIconFieldId,
                                          QStringList(),
                                          record.recordSetName);

        this->recordsController.reparentRecord(record.id, record.parentId);

        for (RecordFieldValueMap::const_iterator itFields = record.fieldValues.cbegin();
             itFields != record.fieldValues.cend();
             ++itFields)
        {
            this->recordsController.updateRecordFieldValue(record.id, itFields.key(), itFields.value());
        }

        this->recordsController.setReadOnly(record.id, record.readOnly);
    }

    // Restore references.
    for (QMap<QVariant, RecordFieldValueMap>::iterator itRecords = this->removedRecordFieldValues.begin();
         itRecords != this->removedRecordFieldValues.end();
         ++itRecords)
    {
        const QVariant& recordId = itRecords.key();
        const RecordFieldValueMap removedValues = itRecords.value();

        for (RecordFieldValueMap::const_iterator itFields = removedValues.cbegin();
             itFields != removedValues.cend();
             ++itFields)
        {
            this->recordsController.updateRecordFieldValue(recordId, itFields.key(), itFields.value());
        }
    }
}

void RemoveRecordCommand::redo()
{
    // Store record data.
    const Record record = this->recordsController.getRecord(this->id);
    this->removedRecords << record;

    const RecordList descendents = this->recordsController.getDescendents(this->id);

    for (RecordList::const_iterator it = descendents.cbegin();
         it != descendents.cend();
         ++it)
    {
        this->removedRecords << *it;
    }

    // Store references of other records pointing to records that are about to be removed.
    this->removedRecordFieldValues.clear();

    RecordList allRecords = this->recordsController.getRecords();

    for (RecordList::const_iterator itAllRecords = allRecords.cbegin();
         itAllRecords != allRecords.cend();
         ++itAllRecords)
    {
        const Record& record = *itAllRecords;

        // Get current record field values.
        const RecordFieldValueMap fieldValues = this->recordsController.getRecordFieldValues(record.id);

        // Prepare map for storing removed record references.
        RecordFieldValueMap removedFieldValues;

        for (RecordFieldValueMap::const_iterator itFields = fieldValues.cbegin();
             itFields != fieldValues.cend();
             ++itFields)
        {
            // Get record field.
            const QString fieldId = itFields.key();
            const FieldDefinition& field = this->fieldDefinitionsController.getFieldDefinition(fieldId);

            // Check if it's a reference field.
            if (this->typesController.isTypeOrDerivedFromType(field.fieldType, BuiltInType::Reference))
            {
                const QString reference = itFields.value().toString();

                // Check if it references any removed record.
                for (RecordList::const_iterator itRemovedRecords = this->removedRecords.cbegin();
                     itRemovedRecords != this->removedRecords.cend();
                     ++itRemovedRecords)
                {
                    const QVariant removedRecordId = (*itRemovedRecords).id;

                    if (reference == removedRecordId.toString())
                    {
                        // Remember field so we can restore the reference later.
                        removedFieldValues.insert(fieldId, removedRecordId);
                    }
                }
            }
        }

        this->removedRecordFieldValues.insert(record.id, removedFieldValues);
    }

    // Remove records recursively.
    this->recordsController.removeRecord(this->id);
}
