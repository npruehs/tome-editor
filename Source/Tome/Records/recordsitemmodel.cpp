#include "recordsitemmodel.h"

using namespace Tome;

RecordsItemModel::RecordsItemModel(QSharedPointer<Tome::Project> project)
    : QStandardItemModel(),
      project(project)
{
    QVector<QSharedPointer<Record> > records = this->project->recordSets[0]->records;

    for (QVector<QSharedPointer<Record> >::iterator it = records.begin();
         it != records.end();
         ++it)
    {
        QSharedPointer<Record> record = *it;
        this->addRecord(record->id);
    }
}

void RecordsItemModel::addRecord(const QString& id)
{
    // Insert item at the end.
    QStandardItem* rootItem = this->invisibleRootItem();
    rootItem->appendRow(new QStandardItem(id));
}
