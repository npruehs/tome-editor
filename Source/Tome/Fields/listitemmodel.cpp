#include "listitemmodel.h"

using namespace Tome;


ListItemModel::ListItemModel(QStringList& list)
    : QStandardItemModel(),
      list(list)
{
}

void ListItemModel::addItem(const QString& item)
{
    this->list.push_back(item);

    QStandardItem* rootItem = this->invisibleRootItem();
    rootItem->appendRow(new QStandardItem(item));
}

void ListItemModel::removeItem(const int index)
{
    this->list.removeAt(index);

    this->removeRows(index, 1, QModelIndex());
}
