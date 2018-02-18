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
    for (QList<QPair<QVariant, RecordFieldValueMap>>::iterator itRecords = this->removedRecordFieldValues.begin();
         itRecords != this->removedRecordFieldValues.end();
         ++itRecords)
    {
        const QVariant& recordId = (*itRecords).first;
        const RecordFieldValueMap removedValues = (*itRecords).second;

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

            // Indicator if we have to store this field
            bool storeFieldValue = false;

            QVariant const &fieldValue = itFields.value();

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
                        storeFieldValue = true;
                    }
                }
            }
            // Check if it is a list type containing references.
            else if (this->typesController.isCustomType(field.fieldType))
            {
                const CustomType& customType = this->typesController.getCustomType(field.fieldType);
                if (customType.isList())
                {
                    if (this->typesController.isTypeOrDerivedFromType(customType.getItemType(), BuiltInType::Reference))
                    {
                        QVariantList fieldValueAsList = fieldValue.toList();
                        for (auto const &listItem : fieldValueAsList)
                        {
                            const QString itemAsReference = listItem.toString();
                            // Check if it references any removed record.
                            for (auto const &removedRecord : this->removedRecords)
                            {
                                QVariant const &removedRecordId = removedRecord.id;

                                if (itemAsReference == removedRecordId.toString())
                                {
                                    // one reference to a removed record inside the list is sufficient,
                                    // so we are finished here
                                    storeFieldValue = true;
                                    break;
                                }
                            }
                            // Do not continue to search the list for matching references
                            if (storeFieldValue)
                            {
                                break;
                            }
                        }
                    }
                }
                // Check if it is a map type containing references.
                else if (customType.isMap())
                {
                    bool const keyIsReference = this->typesController.isTypeOrDerivedFromType(customType.getKeyType(), BuiltInType::Reference);
                    bool const valueIsReference = this->typesController.isTypeOrDerivedFromType(customType.getValueType(), BuiltInType::Reference);
                    if (keyIsReference || valueIsReference)
                    {
                        QVariantMap fieldValueAsMap = fieldValue.toMap();
                        for (QVariantMap::const_iterator itMap = fieldValueAsMap.begin(); fieldValueAsMap.end() != itMap; itMap++)
                        {
                            if (keyIsReference)
                            {
                                QString const &keyAsReference = itMap.key();
                                // Check if it references any removed record.
                                for (auto const &removedRecord : this->removedRecords)
                                {
                                    QVariant const &removedRecordId = removedRecord.id;

                                    if (keyAsReference == removedRecordId.toString())
                                    {
                                        // one reference to a removed record inside the map is sufficient,
                                        // so we are finished here
                                        storeFieldValue = true;
                                        break;
                                    }
                                }
                            }
                            if (!storeFieldValue && valueIsReference)
                            {
                                const QString valueAsReference = itMap.value().toString();
                                // Check if it references any removed record.
                                for (auto const &removedRecord : this->removedRecords)
                                {
                                    QVariant const &removedRecordId = removedRecord.id;

                                    if (valueAsReference == removedRecordId.toString())
                                    {
                                        // one reference to a removed record inside the map is sufficient,
                                        // so we are finished here
                                        storeFieldValue = true;
                                        break;
                                    }
                                }
                            }
                            // Do not continue to search the list for matching references
                            if (storeFieldValue)
                            {
                                break;
                            }
                        }
                    }
                }
            }

            // Remember field so we can restore the reference later.
            if (storeFieldValue)
            {
                removedFieldValues.insert(fieldId, fieldValue);
            }
        }

        this->removedRecordFieldValues << QPair<QVariant, RecordFieldValueMap>(record.id, removedFieldValues);
    }

    // Remove records recursively.
    this->recordsController.removeRecord(this->id);
}
