#include "findusagescontroller.h"

#include "../../Fields/Controller/fielddefinitionscontroller.h"
#include "../../Records/Controller/recordscontroller.h"
#include "../../Types/Controller/typescontroller.h"

using namespace Tome;


FindUsagesController::FindUsagesController(const FieldDefinitionsController& fieldDefinitionsController, const RecordsController& recordsController, const TypesController& typesController)
    : fieldDefinitionsController(fieldDefinitionsController),
      recordsController(recordsController),
      typesController(typesController)
{
}

const SearchResultList FindUsagesController::findUsagesOfType(const QString& typeName)
{
    // Build search result list.
    SearchResultList results;

    // Find all record references.
    const RecordList& records = this->recordsController.getRecords();

    for (int i = 0; i < records.length(); ++i)
    {
        const Record& record = records[i];
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

    emit searchResultChanged("Usages of " + typeName, results);
    return results;
}
