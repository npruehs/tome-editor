#include "recordscontroller.h"

using namespace Tome;


RecordsController::RecordsController()
{
}

const RecordSetList& RecordsController::getRecordSets() const
{
    return *this->model;
}

void RecordsController::setRecordSets(RecordSetList& model)
{
    this->model = &model;
}
