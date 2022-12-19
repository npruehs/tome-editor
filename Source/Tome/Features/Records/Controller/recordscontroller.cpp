#include "recordscontroller.h"

#include <limits>
#include <stdexcept>

#include <QCryptographicHash>
#include <QSet>
#include <QTime>
#include <QUuid>

#include "../../Fields/Controller/fielddefinitionscontroller.h"
#include "../../Fields/Model/fielddefinition.h"
#include "../../Projects/Controller/projectcontroller.h"
#include "../../Projects/Model/recordidtype.h"
#include "../../Types/Controller/typescontroller.h"
#include "../../Types/Model/builtintype.h"
#include "../../../Util/listutils.h"


using namespace Tome;


RecordsController::RecordsController(const FieldDefinitionsController& fieldDefinitionsController,
                                     const ProjectController& projectController,
                                     const TypesController& typesController)
    : fieldDefinitionsController(fieldDefinitionsController),
      projectController(projectController),
      typesController(typesController),
      recordIdGenerator((uint)QTime::currentTime().msec()),
      recordIdDistribution(1, std::numeric_limits<int>::max())
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

const Record RecordsController::addRecord(const QVariant& id,
                                          const QString& displayName,
                                          const QString& editorIconFieldId,
                                          const QStringList& fieldIds,
                                          const QString& recordSetName)
{
    // Create new record.
    Record record = Record();

    // Assign id.
    RecordIdType::RecordIdType recordIdType = this->projectController.getProjectRecordIdType();

    switch (recordIdType)
    {
        case RecordIdType::String:
            record.id = id;
            break;
        case RecordIdType::Integer:
            record.id = id.isNull() ? this->generateIntegerId() : id;
            break;
        case RecordIdType::Uuid:
            record.id = id.isNull() ? this->generateUuid() : id;
            break;
        case RecordIdType::Invalid:
            const QString errorMessage = "Invalid project record id type.";
            qCritical(qUtf8Printable(errorMessage));
            throw std::out_of_range(errorMessage.toStdString());
    }

    qInfo(qUtf8Printable(QString("Adding record %1.").arg(record.id.toString())));

    // Assign display name, icon and record set.
    record.displayName = displayName;
    record.editorIconFieldId = editorIconFieldId;
    record.recordSetName = recordSetName;

    // Set field default values.
    for (QStringList::const_iterator it = fieldIds.cbegin();
         it != fieldIds.cend();
         ++it)
    {
        const QString fieldId = *it;
        const FieldDefinition& field =
                this->fieldDefinitionsController.getFieldDefinition(fieldId);
        record.fieldValues.insert(fieldId, field.defaultValue);
    }

    // Add to record set.
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
            emit this->recordAdded(record.id, displayName, QString());
            return record;
        }
    }

    const QString errorMessage = "Record set not found: " + recordSetName;
    qCritical(qUtf8Printable(errorMessage));
    throw std::out_of_range(errorMessage.toStdString());
}

void RecordsController::addRecordSet(const RecordSet& recordSet)
{
    // Update model.
    this->model->push_back(recordSet);

    // Notify listeners.
    emit this->recordSetsChanged();
}

const QString RecordsController::computeRecordsHash() const
{
    // Prepare MD5 hashing.
    QCryptographicHash hash(QCryptographicHash::Md5);

    // Hash all record files.
    for (int i = 0; i < this->model->size(); ++i)
    {
        const RecordSet& recordSet = this->model->at(i);

        // Hash all records.
        for (int j = 0; j < recordSet.records.size(); ++j)
        {
            const Record& record = recordSet.records[j];
            const RecordFieldValueMap fieldValues = this->getRecordFieldValues(record.id);

            // Hash all fields.
            for (RecordFieldValueMap::const_iterator itFields = fieldValues.cbegin();
                 itFields != fieldValues.cend();
                 ++itFields)
            {
                QString fieldId = itFields.key();
                QVariant fieldValue = itFields.value();

                // Hash field key.
                hash.addData(fieldId.toUtf8());

                // Hash list field items.
                QVariantList list = fieldValue.toList();
                if (!list.isEmpty())
                {
                    for (int k = 0; k < list.count(); ++k)
                    {
                        hash.addData(list[k].toString().toUtf8());
                    }
                    continue;
                }

                // Hash map field items.
                QVariantMap map = fieldValue.toMap();
                if (!map.isEmpty())
                {
                    for (QVariantMap::iterator it = map.begin();
                         it != map.end();
                         ++it)
                    {
                        const QString k = it.key();
                        const QVariant v = it.value();

                        hash.addData(k.toUtf8());
                        hash.addData(v.toString().toUtf8());
                    }
                    continue;
                }

                // Hash field value.
                hash.addData(fieldValue.toString().toUtf8());
            }
        }
    }

    return hash.result().toHex();
}

const Record RecordsController::duplicateRecord(const QVariant& existingRecordId, const QVariant& newRecordId)
{
    qInfo(qUtf8Printable(QString("Duplicating record %1 to %2.")
          .arg(existingRecordId.toString(), newRecordId.toString())));

    // Get record to duplicate.
    const Record& existingRecord = this->getRecord(existingRecordId);

    // Create duplicate.
    Record newRecord = Record();
    newRecord.editorIconFieldId = existingRecord.editorIconFieldId;
    newRecord.parentId = existingRecord.parentId;
    newRecord.fieldValues = existingRecord.fieldValues;
    newRecord.recordSetName = existingRecord.recordSetName;

    // Assign id.
    RecordIdType::RecordIdType recordIdType = this->projectController.getProjectRecordIdType();

    switch (recordIdType)
    {
        case RecordIdType::String:
            newRecord.id = newRecordId;
            newRecord.displayName = newRecordId.toString();
            break;
        case RecordIdType::Integer:
            newRecord.id = this->generateIntegerId();
            newRecord.displayName = existingRecord.displayName + "*";
            break;
        case RecordIdType::Uuid:
            newRecord.id = this->generateUuid();
            newRecord.displayName = existingRecord.displayName + "*";
            break;
        case RecordIdType::Invalid:
            const QString errorMessage = "Invalid project record id type.";
            qCritical(qUtf8Printable(errorMessage));
            throw std::out_of_range(errorMessage.toStdString());
    }

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

const RecordList RecordsController::getAncestors(const QVariant& id) const
{
    RecordList ancestors;

    // Climb hierarchy.
    Record* record = this->getRecordById(id);
    QVariant parentId = record->parentId;

    while (!parentId.isNull() && this->hasRecord(parentId))
    {
        record = this->getRecordById(parentId);
        ancestors.push_back(*record);
        parentId = record->parentId;
    }

    return ancestors;
}

const RecordList RecordsController::getChildren(const QVariant& id) const
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

const RecordList RecordsController::getDescendents(const QVariant& id) const
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

const QVariant RecordsController::getInheritedFieldValue(const QVariant& id, const QString& fieldId) const
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

const RecordFieldValueMap RecordsController::getInheritedFieldValues(const QVariant& id) const
{
    // Resolve parents.
    RecordList ancestors = this->getAncestors(id);

    // Build field value map.
    RecordFieldValueMap fieldValues;

    for (int i = ancestors.count() - 1; i >= 0; --i)
    {
        const Record& ancestor = ancestors.at(i);

        // Combine map.
        for (RecordFieldValueMap::const_iterator it = ancestor.fieldValues.cbegin();
             it != ancestor.fieldValues.cend();
             ++it)
        {
            fieldValues[it.key()] = it.value();
        }
    }

    return fieldValues;
}

const QVariant RecordsController::getParentId(const QVariant& id) const
{
    return this->getRecord(id).parentId;
}

const RecordSetList& RecordsController::getRecordSets() const
{
    return *this->model;
}

const Record& RecordsController::getRecord(const QVariant& id) const
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

const QVariantList RecordsController::getRecordIds() const
{
    RecordList records = this->getRecords();
    QVariantList ids;

    for (int i = 0; i < records.size(); ++i)
    {
        const Record& record = records[i];
        ids << record.id;
    }

    return ids;
}

const QString RecordsController::getRecordEditorIconFieldId(const QVariant& id) const
{
    Record* record = this->getRecordById(id);

    if (!record->editorIconFieldId.isEmpty())
    {
        return record->editorIconFieldId;
    }

    // Resolve parents.
    RecordList ancestors = this->getAncestors(id);

    for (int i = ancestors.count() - 1; i >= 0; --i)
    {
        const Record& ancestor = ancestors.at(i);
        if (!ancestor.editorIconFieldId.isEmpty())
        {
            return ancestor.editorIconFieldId;
        }
    }

    return QString();
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

const RecordFieldValueMap RecordsController::getRecordFieldValues(const QVariant& id) const
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

const QVariant RecordsController::getRootRecordId(const QVariant& id) const
{
    const RecordList ancestors = this->getAncestors(id);
    return ancestors.empty() ? id : ancestors.last().id;
}

bool RecordsController::hasRecord(const QVariant& id) const
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

bool RecordsController::haveTheSameParent(const QVariantList ids) const
{
    if (ids.count() <= 1)
    {
        return true;
    }

    QVariant recordId = ids[0];
    QVariant parentId = this->getParentId(recordId);

    for (int i = 1; i < ids.count(); ++i)
    {
        const QVariant recordId = ids[i];
        const QVariant nextParentId = this->getParentId(recordId);

        // Check parent.
        if (parentId != nextParentId)
        {
            return false;
        }
    }

    return true;
}

bool RecordsController::isAncestorOf(const QVariant& possibleAncestor, const QVariant& recordId) const
{
    // Check if both are valid records.
    if (possibleAncestor.isNull() || recordId.isNull())
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

void RecordsController::removeRecord(const QVariant& recordId)
{
    qInfo(qUtf8Printable(QString("Removing record %1.").arg(recordId.toString())));

    // Remove children.
    RecordList children = this->getChildren(recordId);

    for (int i = 0; i < children.count(); ++i)
    {
        Record& record = children[i];
        this->removeRecord(record.id);
    }

    // Remove references to record.
    //this->updateRecordReferences(recordId, QString());

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

void RecordsController::revertRecord(const QVariant& recordId)
{
    qInfo(qUtf8Printable(QString("Reverting record %1.").arg(recordId.toString())));

    const RecordFieldValueMap& fields = this->getRecordFieldValues(recordId);

    // Revert all fields.
    int i = 0;

    for (RecordFieldValueMap::const_iterator it = fields.cbegin();
         it != fields.cend();
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

void RecordsController::reparentRecord(const QVariant& recordId, const QVariant& newParentId)
{
    qInfo(qUtf8Printable(QString("Reparenting record %1 to %2.")
          .arg(recordId.toString(), newParentId.toString())));

    Record& record = *this->getRecordById(recordId);
    QVariant oldParentId = record.parentId;
    record.parentId = newParentId;
    emit this->recordReparented(recordId, oldParentId, newParentId);
}

void RecordsController::setReadOnly(const QVariant& recordId, const bool readOnly)
{
    Record& record = *this->getRecordById(recordId);
    record.readOnly = readOnly;
}

void RecordsController::setRecordDisplayName(const QVariant& recordId, const QString& displayName)
{
    Record* record = this->getRecordById(recordId);
    const QString oldDisplayName = record->displayName;

    if (oldDisplayName == displayName)
    {
        return;
    }

    qInfo(qUtf8Printable(QString("Updating record %1 display name to %2.")
          .arg(recordId.toString(), displayName)));

    record->displayName = displayName;

    // Notify listeners.
    emit this->recordUpdated(record->id, oldDisplayName, record->editorIconFieldId, record->id, displayName, record->editorIconFieldId);

    // Sort record model to ensure deterministic serialization.
    bool needsSorting = oldDisplayName != displayName;

    if (needsSorting)
    {
        for (RecordSetList::iterator it = this->model->begin();
             it != this->model->end();
             ++it)
        {
            std::sort((*it).records.begin(), (*it).records.end(), recordLessThanDisplayName);
        }
    }
}

void RecordsController::setRecordEditorIconFieldId(const QVariant& recordId, const QString& editorIconFieldId)
{
    Record* record = this->getRecordById(recordId);
    const QString oldEditorIconFieldId = record->editorIconFieldId;

    if (oldEditorIconFieldId == editorIconFieldId)
    {
        return;
    }

    qInfo(qUtf8Printable(QString("Updating record %1 editor icon field id to %2.")
          .arg(recordId.toString(), editorIconFieldId)));

    record->editorIconFieldId = editorIconFieldId;

    // Notify listeners.
    emit this->recordUpdated(record->id, record->displayName, oldEditorIconFieldId, record->id, record->displayName, editorIconFieldId);
}

void RecordsController::setRecordSets(RecordSetList& model)
{
    this->model = &model;

    this->verifyRecordIds();
}

void RecordsController::updateRecord(const QVariant oldId,
                                     const QVariant newId,
                                     const QString newDisplayName,
                                     const QString newEditorIconFieldId,
                                     const QStringList& newFieldIds,
                                     const QString& newRecordSetName)
{
    Record& oldRecord = *this->getRecordById(oldId);
    const QString oldDisplayName = oldRecord.displayName;
    const QString oldEditorIconFieldId = oldRecord.editorIconFieldId;

    if (oldId != newId)
    {
        // Changing the id requires cloning the record, because we
        // to ensure that all references and parent relations are
        // cleanly updated as well.
        this->addRecord(newId, newDisplayName, newEditorIconFieldId, newFieldIds, newRecordSetName);

        Record& newRecord = *this->getRecordById(newId);
        newRecord.fieldValues = oldRecord.fieldValues;
        newRecord.readOnly = oldRecord.readOnly;

        this->reparentRecord(newId, oldRecord.parentId);

        // Update references to record.
        this->updateRecordReferences(oldId, newId);

        // Remove old record.
        this->removeRecord(oldId);
    }

    // Update record itself.
    Record* record = this->getRecordById(newId);

    QVariant recordId = record->id;

    // Update record itself.
    setRecordDisplayName(newId, newDisplayName);

    //When setRecordDisplayName is called it can cause our old pointer to now point to the wrong record due to sorting
    if(record->id != recordId){
        record = this->getRecordById(newId);
    }

    setRecordEditorIconFieldId(newId, newEditorIconFieldId);

    // Move record, if necessary.
    if (record->recordSetName != newRecordSetName)
    {
        this->moveRecordToSet(newId, newRecordSetName);
        record = this->getRecordById(newId);
    }

    // Update record fields.
    const FieldDefinitionList fields = this->fieldDefinitionsController.getFieldDefinitions();
    const RecordFieldValueMap inheritedFieldValues = this->getInheritedFieldValues(record->id);

    for (FieldDefinitionList::const_iterator it = fields.cbegin();
         it != fields.cend();
         ++it)
    {
        const FieldDefinition& field = *it;

        // Skip inherited fields.
        if (inheritedFieldValues.contains(field.id))
        {
            continue;
        }

        // Check if field was added or removed.
        const bool fieldWasEnabled = record->fieldValues.contains(field.id);
        const bool fieldIsEnabled = newFieldIds.contains(field.id);

        if (fieldIsEnabled && !fieldWasEnabled)
        {
            this->addRecordField(record->id, field.id);
        }
        else if (fieldWasEnabled && !fieldIsEnabled)
        {
            this->removeRecordField(record->id, field.id);
        }
    }

    // Notify listeners of changed id and data.
    emit this->recordUpdated(oldId, newDisplayName, newEditorIconFieldId, newId, newDisplayName, newEditorIconFieldId);
}

void RecordsController::updateRecordFieldValue(const QVariant& recordId, const QString& fieldId, const QVariant& fieldValue)
{
    qInfo(qUtf8Printable(QString("Updating record %1 field %2 to value %3.")
          .arg(recordId.toString(), fieldId, fieldValue.toString())));

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
    QVariantList changedRecords;

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

void RecordsController::addRecordField(const QVariant& recordId, const QString& fieldId)
{
    Record& record = *this->getRecordById(recordId);
    const FieldDefinition& field =
            this->fieldDefinitionsController.getFieldDefinition(fieldId);
    record.fieldValues.insert(fieldId, field.defaultValue);

    // Notify listeners.
    emit recordFieldsChanged(recordId);
}

int RecordsController::generateIntegerId()
{
    return recordIdDistribution(recordIdGenerator);
}

const QString RecordsController::generateUuid() const
{
    return QUuid::createUuid().toString().mid(1, 36);
}

Record* RecordsController::getRecordById(const QVariant& id) const
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

    const QString errorMessage = "Record not found: " + id.toString();
    qCritical(qUtf8Printable(errorMessage));
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

void RecordsController::moveRecordToSet(const QVariant& recordId, const QString& recordSetName)
{
    QVariant rid = recordId;

    qInfo(qUtf8Printable(QString("Moving record %1 to set %2.")
          .arg(rid.toString(), recordSetName)));

    Record record = this->getRecord(rid);

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
                if ((*it).id == rid)
                {
                    records.erase(it);
                    break;
                }
            }
        }
    }
}

void RecordsController::removeRecordField(const QVariant& recordId, const QString& fieldId)
{
    qInfo(qUtf8Printable(QString("Removing field %1 from record %2.")
          .arg(fieldId, recordId.toString())));

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

QVariant RecordsController::revertFieldValue(const QVariant& recordId, const QString& fieldId)
{
    qInfo(qUtf8Printable(QString("Reverting field %1 of record %2.")
          .arg(fieldId, recordId.toString())));

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

void RecordsController::updateRecordReferences(const QVariant oldReference, const QVariant newReference)
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
        emit this->progressChanged(tr("Updating references"), record.displayName, i, records.count());

        // Update references.
        const RecordFieldValueMap fieldValues = this->getRecordFieldValues(record.id);

        for (RecordFieldValueMap::const_iterator it = fieldValues.cbegin();
             it != fieldValues.cend();
             ++it)
        {
            const QString fieldId = it.key();
            const FieldDefinition& field = this->fieldDefinitionsController.getFieldDefinition(fieldId);

            if (this->typesController.isTypeOrDerivedFromType(field.fieldType, BuiltInType::Reference))
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
        emit this->progressChanged(tr("Reparenting records"), record.displayName, i, records.count());

        if (record.parentId == oldReference)
        {
            this->reparentRecord(record.id, newReference);
        }
    }

    // Report finish.
    emit this->progressChanged(tr("Reparenting records"), QString(), 1, 1);
}

void RecordsController::verifyRecordIds()
{
    RecordIdType::RecordIdType recordIdType = this->projectController.getProjectRecordIdType();

    switch (recordIdType)
    {
        case RecordIdType::String:
            this->verifyRecordStringIds();
            break;
        case RecordIdType::Integer:
            this->verifyRecordIntegerIds();
            break;
        case RecordIdType::Uuid:
            this->verifyRecordUuids();
            break;
        case RecordIdType::Invalid:
            const QString errorMessage = "Invalid project record id type.";
            qCritical(qUtf8Printable(errorMessage));
            throw std::out_of_range(errorMessage.toStdString());
    }
}

void RecordsController::verifyRecordIntegerIds()
{
    QSet<qlonglong> recordIntegerIds;
    QList<Record*> recordsWithDuplicateIntegerIds;

    for (RecordSet& recordSet : *this->model)
    {
        for (int i = 0; i < recordSet.records.count(); ++i)
        {
            Record& record = recordSet.records[i];
            qlonglong recordIntegerId = record.id.toLongLong();

            emit this->progressChanged(tr("Verifying Record Ids - %1").arg(recordSet.name), record.displayName, i, recordSet.records.count());

            // Verify integer id.
            if (recordIntegerIds.contains(recordIntegerId))
            {
                // Assign new id later, in order to avoid stealing ids from other records.
                recordsWithDuplicateIntegerIds << &record;
            }
            else
            {
                recordIntegerIds.insert(recordIntegerId);
            }

            // Convert to integers.
            record.id = record.id.toLongLong();

            if (!record.parentId.isNull())
            {
                record.parentId = record.parentId.toLongLong();
            }
        }
    }

    // Assign new integer ids where necessary.
    for (Record* record : recordsWithDuplicateIntegerIds)
    {
        qlonglong oldRecordIntegerId = record->id.toLongLong();
        qlonglong newRecordIntegerId = this->generateIntegerId();

        record->id = newRecordIntegerId;

        qWarning(QString("Record %1 had duplicate integer id, assigned new integer id %3.")
                 .arg(QString::number(oldRecordIntegerId), QString::number(newRecordIntegerId))
                 .toUtf8()
                 .constData());
    }

    emit this->progressChanged(QString(), QString(), 1, 1);
}

void RecordsController::verifyRecordStringIds()
{
    // Ensure unique ids for all records.
    QSet<QString> recordStringIds;

    for (RecordSet& recordSet : *this->model)
    {
        for (int i = 0; i < recordSet.records.count(); ++i)
        {
            Record& record = recordSet.records[i];

            emit this->progressChanged(tr("Verifying Record Ids - %1").arg(recordSet.name), record.displayName, i, recordSet.records.count());

            // Verify string id.
            if (recordStringIds.contains(record.id.toString()))
            {
                qCritical(qUtf8Printable(QString("Duplicate record %1. This will lead to undefined behaviour.").arg(record.id.toString())));
            }
            else
            {
                recordStringIds.insert(record.id.toString());
            }
        }
    }

    emit this->progressChanged(QString(), QString(), 1, 1);
}

void RecordsController::verifyRecordUuids()
{
    for (RecordSet& recordSet : *this->model)
    {
        for (int i = 0; i < recordSet.records.count(); ++i)
        {
            Record& record = recordSet.records[i];

            emit this->progressChanged(tr("Verifying Record Ids - %1").arg(recordSet.name), record.displayName, i, recordSet.records.count());

            // Verify UUID.
            if (record.id.isNull())
            {
                record.id = this->generateUuid();
                qWarning(qUtf8Printable(QString("Record %1 had no UUID, assigned %2.").arg(record.displayName, record.id.toString())));
            }
        }
    }

    emit this->progressChanged(QString(), QString(), 1, 1);
}
