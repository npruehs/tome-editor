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

    // Resolve parents.
    QList<Record*> ancestorsAndSelf;
    ancestorsAndSelf.push_front(record);

    QString parentId = record->parentId;

    while (!parentId.isEmpty())
    {
        record = this->getRecordById(parentId);
        ancestorsAndSelf.push_front(record);
        parentId = record->parentId;
    }

    // Build field value map.
    RecordFieldValueMap fieldValues;

    for (int i = 0; i < ancestorsAndSelf.count(); ++i)
    {
        record = ancestorsAndSelf[i];

        // Combine map.
        for (RecordFieldValueMap::iterator it = record->fieldValues.begin();
             it != record->fieldValues.end();
             ++it)
        {
            fieldValues[it.key()] = it.value();
        }
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

    // Climb hierarchy.
    Record* record = this->getRecordById(recordId);

    QString parentId = record->parentId;

    while (!parentId.isEmpty())
    {
        record = this->getRecordById(parentId);

        // Check if ancestor found.
        if (record->id == possibleAncestor)
        {
            return true;
        }

        parentId = record->parentId;
    }

    return false;
}

void RecordsController::removeRecordAt(const int index)
{
    (*this->model)[0].records.removeAt(index);
}

void RecordsController::removeRecordField(const QString& recordId, const QString& fieldId)
{
    Record& record = *this->getRecordById(recordId);
    record.fieldValues.remove(fieldId);
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
    record.fieldValues[fieldId] = fieldValue;
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
