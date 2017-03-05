#include "findrecordcontroller.h"

#include "../../Records/Controller/recordscontroller.h"

using namespace Tome;

FindRecordController::FindRecordController(const Tome::RecordsController& recordsController)
    : recordsController(recordsController)
{
}

const SearchResultList FindRecordController::findRecord(const QString& searchPattern) const
{
    qInfo(QString("Finding records matching pattern %1.").arg(searchPattern).toUtf8().constData());

    // Build search result list.
    SearchResultList results;

    // Find all record references.
    const RecordList& records = this->recordsController.getRecords();

    for (int i = 0; i < records.length(); ++i)
    {
        const Record& record = records[i];

        // Report progress.
        emit this->progressChanged(tr("Searching"), record.id, i, records.length());

        if (record.id.toLower().contains(searchPattern.toLower()) ||
                record.displayName.toLower().contains(searchPattern.toLower()) ||
                (record.integerId != 0 && record.integerId == searchPattern.toLong()) ||
                record.uuid.toLower() == searchPattern.toLower())
        {
            SearchResult result;
            result.content = record.id;
            result.targetSiteId = record.id;
            result.targetSiteType = TargetSiteType::Record;

            results.append(result);
        }
    }

    // Report finish.
    emit this->progressChanged(tr("Searching"), QString(), 1, 1);

    emit searchResultChanged("Find " + searchPattern, results);
    return results;
}
