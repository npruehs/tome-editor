#include "findrecordcontroller.h"

#include "../../Records/Controller/recordscontroller.h"

using namespace Tome;

FindRecordController::FindRecordController(const Tome::RecordsController& recordsController)
    : recordsController(recordsController)
{
}

const SearchResultList FindRecordController::findRecord(const QString& searchPattern)
{
    // Build search result list.
    SearchResultList results;

    // Find all record references.
    const RecordList& records = this->recordsController.getRecords();

    for (int i = 0; i < records.length(); ++i)
    {
        const Record& record = records[i];

        if (record.id.toLower().contains(searchPattern.toLower()) ||
                record.displayName.toLower().contains(searchPattern.toLower()))
        {
            SearchResult result;
            result.content = record.id;
            result.targetSiteId = record.id;
            result.targetSiteType = TargetSiteType::Record;

            results.append(result);
        }
    }

    emit searchResultChanged("Find " + searchPattern, results);
    return results;
}
