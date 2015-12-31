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

void RecordsItemModel::updateRecord(const QString displayName, const QString& newId, const QString& newDisplayName)
{
    // Update record.
    QSharedPointer<Record> record = this->project->getRecordByDisplayName(displayName);

    if (record == 0)
    {
        return;
    }

    record->id = newId;
    record->displayName = newDisplayName;

    // Sort by display name.
    QVector<QSharedPointer<Record> >& records = this->project->recordSets[0]->records;
    std::sort(records.begin(), records.end(), lessThanRecords);

    // Update tree view.
    this->updateItem(displayName, newDisplayName);
}

void RecordsItemModel::removeRecord(const QString& displayName)
{
    // Remove record.
    QSharedPointer<Record> record = this->project->getRecordByDisplayName(displayName);

    if (record == 0)
    {
        return;
    }

    this->project->recordSets[0]->records.removeOne(record);

    // Update tree view.
    this->removeItem(displayName);
}

QStandardItem* RecordsItemModel::findItem(const QString& text)
{
    QList<QStandardItem*> items = this->findItems(text);
    return items.first();
}

void RecordsItemModel::insertItem(const QString& text)
{
    QStandardItem* rootItem = this->invisibleRootItem();
    rootItem->appendRow(new QStandardItem(text));
    rootItem->sortChildren(0);
}

void RecordsItemModel::updateItem(const QString& oldText, const QString& newText)
{
    QStandardItem* item = this->findItem(oldText);
    item->setText(newText);
}

void RecordsItemModel::removeItem(const QString& text)
{
    QStandardItem* item = this->findItem(text);
    QModelIndex index = this->indexFromItem(item);
    this->removeRow(index.row());
}
