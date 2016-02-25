#include "recordscontroller.h"

#include <stdexcept>

#include "../../../Util/listutils.h"


using namespace Tome;


RecordsController::RecordsController(const FieldDefinitionsController& fieldDefinitionsController)
    : fieldDefinitionsController(fieldDefinitionsController)
{
}

const Record RecordsController::addRecord(const QString& id, const QString& displayName)
{
    Record record = Record();
    record.id = id;
    record.displayName = displayName;

    RecordList& records = (*this->model)[0].records;
    int index = findInsertionIndex(records, record, recordLessThanDisplayName);
    records.insert(index, record);

    return record;
}

void RecordsController::addRecordField(const QString& recordId, const QString& fieldId)
{
    Record& record = *this->getRecordById(recordId);
    const FieldDefinition& field =
            this->fieldDefinitionsController.getFieldDefinition(fieldId);
    record.fieldValues.insert(fieldId, field.defaultValue);
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

const QStringList RecordsController::getRecordNames() const
{
    QStringList names;

    for (int i = 0; i < this->model->size(); ++i)
    {
        const RecordSet& recordSet = this->model->at(i);

        for (int j = 0; j < recordSet.records.size(); ++j)
        {
            const Record& record = recordSet.records[j];
            names << record.displayName;
        }
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
    // Remove children.
    RecordList children = this->getChildren(recordId);

    for (int i = 0; i < children.count(); ++i)
    {
        Record& record = children[i];
        this->removeRecord(record.id);
    }

    // Remove record.
    RecordList& records = (*this->model)[0].records;

    for (RecordList::iterator it = records.begin();
         it != records.end();
         ++it)
    {
        Record& record = *it;

        if (record.id == recordId)
        {
            records.erase(it);
            return;
        }
    }
}

void RecordsController::removeRecordField(const QString& recordId, const QString& fieldId)
{
    Record& record = *this->getRecordById(recordId);
    record.fieldValues.remove(fieldId);
}

QVariant RecordsController::revertFieldValue(const QString& recordId, const QString& fieldId)
{
    QVariant inheritedValue = this->getInheritedFieldValue(recordId, fieldId);

    if (inheritedValue != QVariant())
    {
        this->updateRecordFieldValue(recordId, fieldId, inheritedValue);
        return inheritedValue;
    }
    else
    {
        RecordFieldValueMap recordFieldValues = this->getRecordFieldValues(recordId);

        if (recordFieldValues.contains(fieldId))
        {
            return recordFieldValues[fieldId];
        }
        else
        {
            return QVariant();
        }
    }
}

void RecordsController::reparentRecord(const QString& recordId, const QString& newParentId)
{
    Record& record = *this->getRecordById(recordId);
    record.parentId = newParentId;
}

void RecordsController::setRecordSets(RecordSetList& model)
{
    this->model = &model;
}

void RecordsController::updateRecord(const QString& oldId, const QString& newId, const QString& displayName)
{
    Record& record = *this->getRecordById(oldId);

    bool needsSorting = record.displayName != displayName;

    record.id = newId;
    record.displayName = displayName;

    if (needsSorting)
    {
        std::sort((*this->model)[0].records.begin(), (*this->model)[0].records.end(), recordLessThanDisplayName);
    }
}

void RecordsController::updateRecordFieldValue(const QString& recordId, const QString& fieldId, const QVariant& fieldValue)
{
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
    throw std::out_of_range(errorMessage.toStdString());
}
