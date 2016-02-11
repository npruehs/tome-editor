#include "recordscontroller.h"

using namespace Tome;


RecordsController::RecordsController()
{
}

const RecordSetList& RecordsController::getRecordSets() const
{
    return *this->model;
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

void RecordsController::setRecordSets(RecordSetList& model)
{
    this->model = &model;
}
