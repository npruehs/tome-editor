#include "findusagescontroller.h"

#include "../../Fields/Controller/fielddefinitionscontroller.h"
#include "../../Records/Controller/recordscontroller.h"
#include "../../Types/Controller/typescontroller.h"
#include "../../Types/Model/builtintype.h"

using namespace Tome;


FindUsagesController::FindUsagesController(const FieldDefinitionsController& fieldDefinitionsController, const RecordsController& recordsController, const TypesController& typesController)
    : fieldDefinitionsController(fieldDefinitionsController),
      recordsController(recordsController),
      typesController(typesController)
{
}

const SearchResultList FindUsagesController::findUsagesOfField(const QString& fieldId) const
{
    qInfo(QString("Finding usages of field %1.").arg(fieldId).toUtf8().constData());

    // Build search result list.
    SearchResultList results;

    // Find all record references.
    const RecordList& records = this->recordsController.getRecords();

    for (int i = 0; i < records.length(); ++i)
    {
        const Record& record = records[i];

        // Report progress.
        emit this->progressChanged(tr("Searching"), record.id, i, records.length());

        const RecordFieldValueMap& fieldValues = this->recordsController.getRecordFieldValues(record.id);

        for (RecordFieldValueMap::const_iterator it = fieldValues.begin();
             it != fieldValues.end();
             ++it)
        {
            const QString& recordFieldId = it.key();

            if (recordFieldId == fieldId)
            {
                SearchResult result;
                result.content = recordFieldId;
                result.targetSiteId = record.id;
                result.targetSiteType = TargetSiteType::Record;

                results.append(result);
            }
        }
    }

    // Report finish.
    emit this->progressChanged(tr("Searching"), QString(), 1, 1);

    emit searchResultChanged("Usages of " + fieldId, results);
    return results;
}

const SearchResultList FindUsagesController::findUsagesOfRecord(const QString& recordId) const
{
    qInfo(QString("Finding usages of record %1.").arg(recordId).toUtf8().constData());

    // Build search result list.
    SearchResultList results;

    // Find all record references.
    const RecordList& records = this->recordsController.getRecords();

    for (int i = 0; i < records.length(); ++i)
    {
        const Record& record = records[i];

        // Report progress.
        emit this->progressChanged(tr("Searching"), record.id, i, records.length());

        const RecordFieldValueMap& fieldValues = this->recordsController.getRecordFieldValues(record.id);

        for (RecordFieldValueMap::const_iterator it = fieldValues.begin();
             it != fieldValues.end();
             ++it)
        {
            const FieldDefinition& field = this->fieldDefinitionsController.getFieldDefinition(it.key());
            const QVariant& fieldValue = it.value();

            if (this->typesController.isReferenceType(field.fieldType) && fieldValue == recordId)
            {
                SearchResult result;
                result.content = field.id;
                result.targetSiteId = record.id;
                result.targetSiteType = TargetSiteType::Record;

                results.append(result);
            }
        }
    }

    // Report finish.
    emit this->progressChanged(tr("Searching"), QString(), 1, 1);

    emit searchResultChanged("Usages of " + recordId, results);
    return results;
}

const SearchResultList FindUsagesController::findUsagesOfType(const QString& typeName) const
{
    qInfo(QString("Finding usages of type %1.").arg(typeName).toUtf8().constData());

    // Build search result list.
    SearchResultList results;

    // Find all record references.
    const RecordList& records = this->recordsController.getRecords();

    for (int i = 0; i < records.length(); ++i)
    {
        const Record& record = records[i];

        // Report progress.
        emit this->progressChanged(tr("Searching"), record.id, i, records.length());

        const RecordFieldValueMap& fieldValues = this->recordsController.getRecordFieldValues(record.id);

        for (RecordFieldValueMap::const_iterator it = fieldValues.begin();
             it != fieldValues.end();
             ++it)
        {
            const FieldDefinition& field = this->fieldDefinitionsController.getFieldDefinition(it.key());

            if (field.fieldType == typeName)
            {
                SearchResult result;
                result.content = field.id;
                result.targetSiteId = record.id;
                result.targetSiteType = TargetSiteType::Record;

                results.append(result);
            }
        }
    }

    // Report finish.
    emit this->progressChanged(tr("Searching"), QString(), 1, 1);

    emit searchResultChanged("Usages of " + typeName, results);
    return results;
}
