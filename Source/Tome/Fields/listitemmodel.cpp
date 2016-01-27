#include "listitemmodel.h"

using namespace Tome;


ListItemModel::ListItemModel(QStringList& list)
    : QStandardItemModel(),
      list(list)
{
}

void ListItemModel::addItem(const QString& item)
{
    this->addItem(this->list.size(), item);
}

void ListItemModel::addItem(int index, const QString& item)
{
    this->list.insert(index, item);

    QStandardItem* rootItem = this->invisibleRootItem();
    rootItem->insertRow(index, new QStandardItem(item));
}

QString ListItemModel::removeItem(const int index)
{
    QString item = this->list.takeAt(index);
    this->removeRows(index, 1, QModelIndex());
    return item;
}

void ListItemModel::moveItemUp(const int index)
{
    if (index < 1)
    {
        return;
    }

    QString item = this->removeItem(index);
    this->addItem(index - 1, item);
}

void ListItemModel::moveItemDown(const int index)
{
    if (index > this->list.count() - 2)
    {
        return;
    }

    QString item = this->removeItem(index);
    this->addItem(index + 1, item);
}
