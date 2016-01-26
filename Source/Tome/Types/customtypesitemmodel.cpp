#include "customtypesitemmodel.h"

#include "../Types/builtintype.h"

using namespace Tome;


bool lessThanCustomTypes(QSharedPointer<CustomType> e1, QSharedPointer<CustomType> e2)
{
    return e1->name.toLower() < e2->name.toLower();
}


CustomTypesItemModel::CustomTypesItemModel(QSharedPointer<Tome::Project> project)
    : QStandardItemModel(),
      project(project)
{
    QVector<QSharedPointer<CustomType> >& types = this->project->types;

    for (QVector<QSharedPointer<CustomType> >::iterator it = types.begin();
         it != types.end();
         ++it)
    {
        QSharedPointer<CustomType> type = *it;
        this->insertItem(type->name);
    }
}

void CustomTypesItemModel::addEnumeration(const QString& name, const QStringList& enumeration)
{
    // Add type.
    QSharedPointer<CustomType> newType = QSharedPointer<CustomType>::create();
    newType->name = name;
    newType->baseType = BuiltInType::String;
    newType->setEnumeration(enumeration);

    this->project->types.push_back(newType);

    // Sort by name.
    std::sort(this->project->types.begin(), this->project->types.end(), lessThanCustomTypes);

    // Insert item.
    this->insertItem(newType->name);
}

void CustomTypesItemModel::addList(const QString& name, const QString& itemType)
{
    // Add type.
    QSharedPointer<CustomType> newType = QSharedPointer<CustomType>::create();
    newType->name = name;
    newType->baseType = BuiltInType::String;
    newType->setItemType(itemType);

    this->project->types.push_back(newType);

    // Sort by name.
    std::sort(this->project->types.begin(), this->project->types.end(), lessThanCustomTypes);

    // Insert item.
    this->insertItem(newType->name);
}

void CustomTypesItemModel::updateEnumeration(const int index, const QString& name, const QStringList& enumeration)
{
    // Get custom type.
    QSharedPointer<CustomType> type =  this->project->types.at(index);
    QString oldName = type->name;

    // Update type.
    type->name = name;
    type->setEnumeration(enumeration);

    // Update item.
    this->updateItem(oldName, name);
}

void CustomTypesItemModel::removeCustomType(const int index)
{
    // Remove type.
    this->project->types.removeAt(index);

    // Remove item.
    this->removeRow(index);
}

void CustomTypesItemModel::insertItem(const QString& text)
{
    QStandardItem* rootItem = this->invisibleRootItem();
    rootItem->appendRow(new QStandardItem(text));
    rootItem->sortChildren(0);
}

void CustomTypesItemModel::updateItem(const QString& oldText, const QString& newText)
{
    QStandardItem* item = this->findItems(oldText).first();
    QModelIndex index = this->indexFromItem(item);
    this->setData(index, newText, Qt::DisplayRole);
}
