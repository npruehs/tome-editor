#include "recordsitemmodel.h"

using namespace Tome;


bool lessThanRecords(const QSharedPointer<Record> e1, const QSharedPointer<Record> e2)
{
    return e1->displayName.toLower() < e2->displayName.toLower();
}


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
        this->insertItem(record->displayName);
    }
}

void RecordsItemModel::addRecord(const QString& id, const QString& displayName)
{
    // Add record.
    QSharedPointer<Record> record = QSharedPointer<Record>::create();
    record->id = id;
    record->displayName = displayName;

    QVector<QSharedPointer<Record> >& records = this->project->recordSets[0]->records;
    records.push_back(record);

    // Sort by display name.
    std::sort(records.begin(), records.end(), lessThanRecords);

    // Insert tree view item.
    this->insertItem(displayName);
}

void RecordsItemModel::insertItem(const QString& text)
{
    QStandardItem* rootItem = this->invisibleRootItem();
    rootItem->appendRow(new QStandardItem(text));
    rootItem->sortChildren(0);
}
