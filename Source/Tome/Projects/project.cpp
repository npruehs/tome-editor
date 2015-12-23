#include "project.h"

using namespace Tome;

Project::Project()
{

}

QSharedPointer<Record> Project::getRecordByDisplayName(const QString& displayName)
{
    for (QVector<QSharedPointer<RecordSet> >::iterator itRecordSets = this->recordSets.begin();
         itRecordSets != this->recordSets.end();
         ++itRecordSets)
    {
        QSharedPointer<RecordSet> recordSet = *itRecordSets;

        for (QVector<QSharedPointer<Record> >::iterator itRecords = recordSet->records.begin();
             itRecords != recordSet->records.end();
             ++itRecords)
        {
            QSharedPointer<Record> record = *itRecords;

            if (record->displayName == displayName)
            {
                return record;
            }
        }
    }

    return QSharedPointer<Record>(0);
}
