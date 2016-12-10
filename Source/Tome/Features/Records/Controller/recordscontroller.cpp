#include "recordscontroller.h"

#include <stdexcept>

#include "../../Fields/Controller/fielddefinitionscontroller.h"
#include "../../Fields/Model/fielddefinition.h"
#include "../../Types/Controller/typescontroller.h"
#include "../../Types/Model/builtintype.h"
#include "../../../Util/listutils.h"


using namespace Tome;


RecordsController::RecordsController(const FieldDefinitionsController& fieldDefinitionsController, const TypesController& typesController)
    : fieldDefinitionsController(fieldDefinitionsController),
      typesController(typesController)
{
    connect(&this->fieldDefinitionsController,
            SIGNAL(fieldDefinitionAdded(const Tome::FieldDefinition&)),
            SLOT(onFieldAdded(const Tome::FieldDefinition&)));

    connect(&this->fieldDefinitionsController,
            SIGNAL(fieldDefinitionRemoved(const Tome::FieldDefinition&)),
            SLOT(onFieldRemoved(const Tome::FieldDefinition&)));

    connect(&this->fieldDefinitionsController,
            SIGNAL(fieldDefinitionUpdated(const Tome::FieldDefinition&, const Tome::FieldDefinition&)),
            SLOT(onFieldUpdated(const Tome::FieldDefinition&, const Tome::FieldDefinition&)));
}

const Record RecordsController::addRecord(const QString& id, const QString& displayName, const QStringList& fieldIds, const QString& recordSetName)
{
    qInfo(QString("Adding record %1.").arg(id).toUtf8().constData());

    Record record = Record();
    record.id = id;
    record.displayName = displayName;
    record.recordSetName = recordSetName;

    for (QStringList::const_iterator it = fieldIds.begin();
         it != fieldIds.end();
         ++it)
    {
        const QString fieldId = *it;
        const FieldDefinition& field =
                this->fieldDefinitionsController.getFieldDefinition(fieldId);
        record.fieldValues.insert(fieldId, field.defaultValue);
    }

    for (RecordSetList::iterator it = this->model->begin();
         it != this->model->end();
         ++it)
    {
        RecordSet& recordSet = *it;

        if (recordSet.name == recordSetName)
        {
            RecordList& records = recordSet.records;
            int index = findInsertionIndex(records, record, recordLessThanDisplayName);
            records.insert(index, record);
            emit this->recordAdded(id, displayName, QString());
            return record;
        }
    }

    const QString errorMessage = "Record set not found: " + recordSetName;
    qCritical(errorMessage.toUtf8().constData());
    throw std::out_of_range(errorMessage.toStdString());
}

void RecordsController::addRecordSet(const RecordSet& recordSet)
{
    // Update model.
    this->model->push_back(recordSet);

    // Notify listeners.
    emit this->recordSetsChanged();
}

const Record RecordsController::duplicateRecord(const QString& existingRecordId, const QString& newRecordId)
{
    qInfo(QString("Duplicating record %1 to %2.").arg(existingRecordId, newRecordId).toUtf8().constData());

    // Get record to duplicate.
    const Record& existingRecord = this->getRecord(existingRecordId);

    // Create duplicate.
    Record newRecord = Record();
    newRecord.id = newRecordId;
    newRecord.displayName = newRecordId;
    newRecord.parentId = existingRecord.parentId;
    newRecord.fieldValues = existingRecord.fieldValues;
    newRecord.recordSetName = existingRecord.recordSetName;

    // Add new record.
    int recordSetIndex = 0;
    for (int i = 0; this->model->size() > i; ++i)
    {
        if ((*this->model)[i].name == existingRecord.recordSetName)
        {
            recordSetIndex = i;
            break;
        }
    }
    RecordList& records = (*this->model)[recordSetIndex].records;
    int index = findInsertionIndex(records, newRecord, recordLessThanDisplayName);
    records.insert(index, newRecord);
    emit this->recordAdded(newRecord.id, newRecord.displayName, newRecord.parentId);

    return newRecord;
}

const RecordList RecordsController::getAncestors(const QString& id) const
{
    RecordList ancestors;

    // Climb hierarchy.
    Record* record = this->getRecordById(id);
    QString parentId = record->parentId;

    while (!parentId.isEmpty())
    {
        record = this->getRecordById(parentId);
        ancestors.push_back(*record);
        parentId = record->parentId;
    }

    return ancestors;
}

const RecordList RecordsController::getChildren(const QString& id) const
{
    RecordList children;

    for (int i = 0; i < this->model->size(); ++i)
    {
        const RecordSet& recordSet = this->model->at(i);

        for (int j = 0; j < recordSet.records.size(); ++j)
        {
            const Record& record = recordSet.records[j];

            if (record.parentId == id)
            {
                children.append(record);
            }
        }
    }

    return children;
}

const RecordList RecordsController::getDescendents(const QString& id) const
{
    RecordList descendents;

    // Climb hierarchy.
    RecordList records = this->getRecords();

    for (int i = 0; i < records.count(); ++i)
    {
        // Check children.
        const Record& record = records.at(i);

        if (record.parentId == id)
        {
            descendents << record;

            // Recursively check descendants.
            RecordList childrenOfChild = this->getDescendents(record.id);

            for (int j = 0; j < childrenOfChild.count(); ++j)
            {
                const Record& child = records.at(j);
                descendents << child;
            }
        }
    }

    return descendents;
}

const QVariant RecordsController::getInheritedFieldValue(const QString& id, const QString& fieldId) const
{
    RecordList ancestors = this->getAncestors(id);

    for (int i = 0; i < ancestors.count(); ++i)
    {
        const Record& ancestor = ancestors.at(i);

        if (ancestor.fieldValues.contains(fieldId))
        {
            return ancestor.fieldValues[fieldId];
        }
    }

    return QVariant();
}

const RecordFieldValueMap RecordsController::getInheritedFieldValues(const QString& id) const
{
    // Resolve parents.
    RecordList ancestors = this->getAncestors(id);

    // Build field value map.
    RecordFieldValueMap fieldValues;

    for (int i = 0; i < ancestors.count(); ++i)
    {
        const Record& ancestor = ancestors.at(i);

        // Combine map.
        for (RecordFieldValueMap::const_iterator it = ancestor.fieldValues.begin();
             it != ancestor.fieldValues.end();
             ++it)
        {
            fieldValues[it.key()] = it.value();
        }
    }

    return fieldValues;
}

const RecordSetList& RecordsController::getRecordSets() const
{
    return *this->model;
}

const Record& RecordsController::getRecord(const QString& id) const
{
    return *this->getRecordById(id);
}

const RecordList RecordsController::getRecords() const
{
    RecordList records;

    for (int i = 0; i < this->model->size(); ++i)
    {
        const RecordSet& recordSet = this->model->at(i);

        for (int j = 0; j < recordSet.records.size(); ++j)
        {
            records << recordSet.records[j];
        }
    }

    return records;
}

const QStringList RecordsController::getRecordIds() const
{
    RecordList records = this->getRecords();
    QStringList ids;

    for (int i = 0; i < records.size(); ++i)
    {
        const Record& record = records[i];
        ids << record.id;
    }

    return ids;
}

const QStringList RecordsController::getRecordNames() const
{
    RecordList records = this->getRecords();
    QStringList names;

    for (int i = 0; i < records.size(); ++i)
    {
        const Record& record = records[i];
        names << record.displayName;
    }

    return names;
}

const QStringList RecordsController::getRecordSetNames() const
{
    QStringList names;

    for (int i = 0; i < this->model->size(); ++i)
    {
        const RecordSet& recordSet = this->model->at(i);
        names << recordSet.name;
    }

    return names;
}

const RecordFieldValueMap RecordsController::getRecordFieldValues(const QString& id) const
{
    Record* record = this->getRecordById(id);

    // Get inherited values.
    RecordFieldValueMap fieldValues = this->getInheritedFieldValues(id);

    // Override inherited values.
    for (RecordFieldValueMap::iterator it = record->fieldValues.begin();
         it != record->fieldValues.end();
         ++it)
    {
        fieldValues[it.key()] = it.value();
    }

    return fieldValues;
}

bool RecordsController::hasRecord(const QString& id) const
{
    for (int i = 0; i < this->model->size(); ++i)
    {
        RecordSet& recordSet = (*this->model)[i];

        for (int j = 0; j < recordSet.records.size(); ++j)
        {
            Record& record = recordSet.records[j];

            if (record.id == id)
            {
                return true;
            }
        }
    }

    return false;
}

int RecordsController::indexOf(const Record& record) const
{
    return this->model->at(0).records.indexOf(record);
}

bool RecordsController::isAncestorOf(const QString& possibleAncestor, const QString& recordId) const
{
    // Check if both are valid records.
    if (possibleAncestor.isEmpty() || recordId.isEmpty())
    {
        return false;
    }

    RecordList ancestors = this->getAncestors(recordId);

    for (int i = 0; i < ancestors.count(); ++i)
    {
        if (ancestors[i].id == possibleAncestor)
        {
            return true;
        }
    }

    return false;
}

void RecordsController::removeRecord(const QString& recordId)
{
    qInfo(QString("Removing record %1.").arg(recordId).toUtf8().constData());

    // Remove children.
    RecordList children = this->getChildren(recordId);

    for (int i = 0; i < children.count(); ++i)
    {
        Record& record = children[i];
        this->removeRecord(record.id);
    }

    // Remove references to record.
    this->updateRecordReferences(recordId, QString());

    // Remove record.
    for (RecordSetList::iterator itSets = this->model->begin();
         itSets != this->model->end();
         ++itSets)
    {
        RecordList& records = (*itSets).records;

        for (RecordList::iterator it = records.begin();
             it != records.end();
             ++it)
        {
            Record& record = *it;

            if (record.id == recordId)
            {
                records.erase(it);
                emit this->recordRemoved(recordId);
                return;
            }
        }
    }
}

void RecordsController::removeRecordSet(const QString& name)
{
    for (RecordSetList::iterator it = this->model->begin();
         it != this->model->end();
         ++it)
    {
        if ((*it).name == name)
        {
            // Update model.
            this->model->erase(it);

            // Notify listeners.
            emit this->recordSetsChanged();
            return;
        }
    }
}

void RecordsController::revertRecord(const QString& recordId)
{
    qInfo(QString("Reverting record %1.").arg(recordId).toUtf8().constData());

    const RecordFieldValueMap& fields = this->getRecordFieldValues(recordId);

    // Revert all fields.
    int i = 0;

    for (RecordFieldValueMap::const_iterator it = fields.begin();
         it != fields.end();
         ++it, ++i)
    {
        const QString& fieldId = it.key();

        // Report progress.
        emit this->progressChanged(tr("Reverting fields"), fieldId, i, fields.count());

        this->revertFieldValue(recordId, fieldId);
    }

    // Report finish.
    emit this->progressChanged(tr("Reverting fields"), QString(), 1, 1);
}

void RecordsController::reparentRecord(const QString& recordId, const QString& newParentId)
{
    qInfo(QString("Reparenting record %1 to %2.").arg(recordId, newParentId).toUtf8().constData());

    Record& record = *this->getRecordById(recordId);
    QString oldParentId = record.parentId;
    record.parentId = newParentId;
    emit this->recordReparented(recordId, oldParentId, newParentId);
}

void RecordsController::setReadOnly(const QString& recordId, const bool readOnly)
{
    Record& record = *this->getRecordById(recordId);
    record.readOnly = readOnly;
}

void RecordsController::setRecordSets(RecordSetList& model)
{
    this->model = &model;
}

void RecordsController::updateRecord(const QString oldId, const QString newId, const QString newDisplayName, const QStringList& fieldIds, const QString& recordSetName)
{
    // Update references to record.
    this->updateRecordReferences(oldId, newId);

    // Update record itself.
    Record& record = *this->getRecordById(oldId);
    const QString oldDisplayName = record.displayName;

    bool needsSorting = oldDisplayName != newDisplayName;

    record.id = newId;
    record.displayName = newDisplayName;

    // Update record fields.
    const FieldDefinitionList fields = this->fieldDefinitionsController.getFieldDefinitions();
    const RecordFieldValueMap inheritedFieldValues = this->getInheritedFieldValues(record.id);

    for (FieldDefinitionList::const_iterator it = fields.begin();
         it != fields.end();
         ++it)
    {
        const FieldDefinition& field = *it;

        // Skip inherited fields.
        if (inheritedFieldValues.contains(field.id))
        {
            continue;
        }

        // Check if field was added or removed.
        const bool fieldWasEnabled = record.fieldValues.contains(field.id);
        const bool fieldIsEnabled = fieldIds.contains(field.id);

        if (fieldIsEnabled && !fieldWasEnabled)
        {
            this->addRecordField(record.id, field.id);
        }
        else if (fieldWasEnabled && !fieldIsEnabled)
        {
            this->removeRecordField(record.id, field.id);
        }
    }

    // Move record, if necessary.
    if (record.recordSetName != recordSetName)
    {
        this->moveRecordToSet(record.id, recordSetName);
    }

    if (needsSorting)
    {
        for (RecordSetList::iterator it = this->model->begin();
             it != this->model->end();
             ++it)
        {
            std::sort((*it).records.begin(), (*it).records.end(), recordLessThanDisplayName);
        }
    }

    // Notify listeners.
    emit this->recordUpdated(oldId, oldDisplayName, newId, newDisplayName);
}

void RecordsController::updateRecordFieldValue(const QString& recordId, const QString& fieldId, const QVariant& fieldValue)
{
    qInfo(QString("Updating record %1 field %2 to value %3.").arg(recordId, fieldId, fieldValue.toString()).toUtf8().constData());

    Record& record = *this->getRecordById(recordId);

    // Check if equals inherited field value.
    QVariant inheritedValue = this->getInheritedFieldValue(recordId, fieldId);

    if (inheritedValue == fieldValue)
    {
        record.fieldValues.remove(fieldId);
    }
    else
    {
        record.fieldValues[fieldId] = fieldValue;
    }

    // Notify listeners.
    emit recordFieldsChanged(recordId);
}

void RecordsController::onFieldAdded(const FieldDefinition& fieldDefinition)
{
    this->moveFieldToComponent(fieldDefinition.id, QString(), fieldDefinition.component);
}

void RecordsController::onFieldRemoved(const FieldDefinition& fieldDefinition)
{
    QList<QString> changedRecords;

    // Remove field from all records first, before notifying any listeners.
    // Notifying them earlier can cause inconsistent behaviour due to
    // records inheriting fields from parents who don't have the respective
    // field removed yet.
    for (int i = 0; i < this->model->size(); ++i)
    {
        RecordSet& recordSet = (*this->model)[i];

        for (int j = 0; j < recordSet.records.size(); ++j)
        {
            Record& record = recordSet.records[j];
            if (record.fieldValues.remove(fieldDefinition.id) > 0)
            {
                changedRecords << record.id;
            }
        }
    }

    // Notify listeners.
    for (int i = 0; i < changedRecords.count(); ++i)
    {
        emit recordFieldsChanged(changedRecords[i]);
    }
}

void RecordsController::onFieldUpdated(const FieldDefinition& oldFieldDefinition, const FieldDefinition& newFieldDefinition)
{
    this->renameRecordField(oldFieldDefinition.id, newFieldDefinition.id);
    this->moveFieldToComponent(newFieldDefinition.id, oldFieldDefinition.component, newFieldDefinition.component);
}

void RecordsController::addRecordField(const QString& recordId, const QString& fieldId)
{
    Record& record = *this->getRecordById(recordId);
    const FieldDefinition& field =
            this->fieldDefinitionsController.getFieldDefinition(fieldId);
    record.fieldValues.insert(fieldId, field.defaultValue);

    // Notify listeners.
    emit recordFieldsChanged(recordId);
}

Record* RecordsController::getRecordById(const QString& id) const
{
    for (int i = 0; i < this->model->size(); ++i)
    {
        RecordSet& recordSet = (*this->model)[i];

        for (int j = 0; j < recordSet.records.size(); ++j)
        {
            Record& record = recordSet.records[j];

            if (record.id == id)
            {
                return &record;
            }
        }
    }

    const QString errorMessage = "Record not found: " + id;
    qCritical(errorMessage.toUtf8().constData());
    throw std::out_of_range(errorMessage.toStdString());
}

void RecordsController::moveFieldToComponent(const QString& fieldId, const QString& oldComponent, const QString& newComponent)
{
    if (oldComponent == newComponent)
    {
        return;
    }

    // Get all fields that belong to the old and new component.
    const FieldDefinitionList& fields = this->fieldDefinitionsController.getFieldDefinitions();

    FieldDefinitionList oldComponentFields;
    FieldDefinitionList newComponentFields;

    for (int i = 0; i < fields.size(); ++i)
    {
        const FieldDefinition& field = fields[i];

        if (field.id == fieldId)
        {
            continue;
        }

        if (field.component == oldComponent)
        {
            oldComponentFields.push_back(field);
        }
        else if (field.component == newComponent)
        {
            newComponentFields.push_back(field);
        }
    }

    // Check all records for their current components.
    for (int i = 0; i < this->model->size(); ++i)
    {
        RecordSet& recordSet = (*this->model)[i];

        for (int j = 0; j < recordSet.records.size(); ++j)
        {
            Record& record = recordSet.records[j];

            if (!oldComponent.isEmpty() && !oldComponentFields.empty())
            {
                // If record has all fields of old component, remove field.
                bool hasAllFieldsOfOldComponent = true;

                for (int i = 0; i < oldComponentFields.size(); ++i)
                {
                    const FieldDefinition& field = oldComponentFields[i];

                    if (!record.fieldValues.contains(field.id))
                    {
                        hasAllFieldsOfOldComponent = false;
                        break;
                    }
                }

                if (hasAllFieldsOfOldComponent)
                {
                    this->removeRecordField(record.id, fieldId);
                }
            }

            if (!newComponent.isEmpty() && !newComponentFields.empty())
            {
                // If record has all fields of new component, add field.
                bool hasAllFieldsOfNewComponent = true;

                for (int i = 0; i < newComponentFields.size(); ++i)
                {
                    const FieldDefinition& field = newComponentFields[i];

                    if (!record.fieldValues.contains(field.id))
                    {
                        hasAllFieldsOfNewComponent = false;
                        break;
                    }
                }

                if (hasAllFieldsOfNewComponent)
                {
                    this->addRecordField(record.id, fieldId);
                }
            }
        }
    }
}

void RecordsController::moveRecordToSet(const QString& recordId, const QString& recordSetName)
{
    qInfo(QString("Moving record %1 to set %2.").arg(recordId, recordSetName).toUtf8().constData());

    Record record = this->getRecord(recordId);

    for (RecordSetList::iterator itSets = this->model->begin();
         itSets != this->model->end();
         ++itSets)
    {
        RecordSet& recordSet = (*itSets);
        RecordList& records = recordSet.records;

        // Check if should add record.
        if (recordSet.name == recordSetName)
        {
            int index = findInsertionIndex(records, record, recordLessThanDisplayName);
            record.recordSetName = recordSetName;
            records.insert(index, record);
            continue;
        }
        else
        {
            // Check if should remove record.
            for (RecordList::iterator it = records.begin();
                 it != records.end();
                 ++it)
            {
                if ((*it).id == recordId)
                {
                    records.erase(it);
                    break;
                }
            }
        }
    }
}

void RecordsController::removeRecordField(const QString& recordId, const QString& fieldId)
{
    qInfo(QString("Removing field %1 from record %2.").arg(fieldId, recordId).toUtf8().constData());

    Record& record = *this->getRecordById(recordId);
    record.fieldValues.remove(fieldId);

    // Remove inherited fields.
    RecordList descendants = this->getDescendents(recordId);

    for (int i = 0; i < descendants.count(); ++i)
    {
        Record& record = descendants[i];
        this->removeRecordField(record.id, fieldId);
    }

    // Notify listeners.
    emit recordFieldsChanged(recordId);
}

void RecordsController::renameRecordField(const QString oldFieldId, const QString newFieldId)
{
    for (int i = 0; i < this->model->size(); ++i)
    {
        RecordSet& recordSet = (*this->model)[i];

        for (int j = 0; j < recordSet.records.size(); ++j)
        {
            Record& record = recordSet.records[j];

            if (record.fieldValues.contains(oldFieldId))
            {
                const QVariant fieldValue = record.fieldValues[oldFieldId];
                record.fieldValues.remove(oldFieldId);
                record.fieldValues.insert(newFieldId, fieldValue);

                // Notify listeners.
                emit recordFieldsChanged(record.id);
            }
        }
    }
}

QVariant RecordsController::revertFieldValue(const QString& recordId, const QString& fieldId)
{
    qInfo(QString("Reverting field %1 of record %2.").arg(fieldId, recordId).toUtf8().constData());

    // Check if there's anything to revert to.
    QVariant valueToRevertTo = this->getInheritedFieldValue(recordId, fieldId);

    if (valueToRevertTo == QVariant())
    {
        // Revert to field default value.
        const FieldDefinition& field =
                this->fieldDefinitionsController.getFieldDefinition(fieldId);

        valueToRevertTo = field.defaultValue;
    }

    // Revert field value.
    this->updateRecordFieldValue(recordId, fieldId, valueToRevertTo);

    // Return reverted value.
    return valueToRevertTo;
}

void RecordsController::updateRecordReferences(const QString oldReference, const QString newReference)
{
    if (oldReference == newReference)
    {
        return;
    }

    RecordList records = this->getRecords();

    // First pass: update reference fields
    for (int i = 0; i < records.count(); ++i)
    {
        const Record& record = records.at(i);

        // Report progress.
        emit this->progressChanged(tr("Updating references"), record.id, i, records.count());

        // Update references.
        const RecordFieldValueMap fieldValues = this->getRecordFieldValues(record.id);

        for (RecordFieldValueMap::const_iterator it = fieldValues.begin();
             it != fieldValues.end();
             ++it)
        {
            const QString fieldId = it.key();
            const FieldDefinition& field = this->fieldDefinitionsController.getFieldDefinition(fieldId);

            if (this->typesController.isReferenceType(field.fieldType))
            {
                const QString reference = it.value().toString();

                if (reference == oldReference)
                {
                    this->updateRecordFieldValue(record.id, fieldId, newReference);
                }
            }
        }
    }

    // Second pass: Update parents.
    for (int i = 0; i < records.count(); ++i)
    {
        const Record& record = records.at(i);

        // Report progress.
        emit this->progressChanged(tr("Reparenting records"), record.id, i, records.count());

        if (record.parentId == oldReference)
        {
            this->reparentRecord(record.id, newReference);
        }
    }

    // Report finish.
    emit this->progressChanged(tr("Reparenting records"), QString(), 1, 1);
}
