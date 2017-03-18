#include "findrecordcontroller.h"

#include "../../Records/Controller/recordscontroller.h"

using namespace Tome;

FindRecordController::FindRecordController(const Tome::RecordsController& recordsController)
    : recordsController(recordsController)
{
}

const SearchResultList FindRecordController::findRecord(const QString& searchPattern) const
{
    qInfo(qUtf8Printable(QString("Finding records matching pattern %1.").arg(searchPattern)));

    // Build search result list.
    SearchResultList results;

    // Find all record references.
    const RecordList& records = this->recordsController.getRecords();

    for (int i = 0; i < records.length(); ++i)
    {
        const Record& record = records[i];

        // Report progress.
        emit this->progressChanged(tr("Searching"), record.displayName, i, records.length());

        if (record.id.toString().toLower().contains(searchPattern.toLower()) ||
                record.displayName.toLower().contains(searchPattern.toLower()))
        {
            SearchResult result;
            result.content = QString("%1 (%2)").arg(record.displayName, record.id.toString());
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
