#include "componentsitemmodel.h"

using namespace Tome;


bool lessThanComponents(const QString e1, const QString e2)
{
    return e1.toLower() < e2.toLower();
}


ComponentsItemModel::ComponentsItemModel(QSharedPointer<Tome::Project> project)
    : QStandardItemModel(),
      project(project)
{
    QVector<QString>& components = this->project->components;

    for (QVector<QString>::iterator it = components.begin();
         it != components.end();
         ++it)
    {
        QString& component = *it;
        this->insertItem(component);
    }
}

void ComponentsItemModel::addComponent(const QString& componentName)
{
    QVector<QString>& components = this->project->components;

    // Add component.
    components.push_back(componentName);

    // Sort by name.
    std::sort(components.begin(), components.end(), lessThanComponents);

    // Insert item.
    this->insertItem(componentName);
}

void ComponentsItemModel::removeComponent(const int index)
{
    // Remove component.
    this->project->components.removeAt(index);

    // Remove item.
    this->removeRows(index, 1, QModelIndex());
}

void ComponentsItemModel::removeComponent(const QString& componentName)
{
    // Remove component.
    this->project->components.removeOne(componentName);

    // Remove item.
    this->removeItem(componentName);
}

void ComponentsItemModel::insertItem(const QString& text)
{
    QStandardItem* rootItem = this->invisibleRootItem();
    rootItem->appendRow(new QStandardItem(text));
    //rootItem->sortChildren(0);
}

void ComponentsItemModel::removeItem(const QString& text)
{
    QStandardItem* item = this->findItems(text).first();
    QModelIndex index = this->indexFromItem(item);
    this->removeRow(index.row());
}
