#include "customtypestablemodel.h"

#include "../Types/builtintype.h"

using namespace Tome;


bool lessThanCustomTypes(QSharedPointer<CustomType> e1, QSharedPointer<CustomType> e2)
{
    return e1->name.toLower() < e2->name.toLower();
}


CustomTypesTableModel::CustomTypesTableModel(QObject *parent, QSharedPointer<Tome::Project> project)
    : QAbstractTableModel(parent),
      project(project)
{
}

int CustomTypesTableModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return this->project->types.length();
}

int CustomTypesTableModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return 3;
}

QVariant CustomTypesTableModel::data(const QModelIndex& index, int role) const
{
    // Validate index.
    if (!index.isValid())
    {
        return QVariant();
    }

    if (index.row() >= this->rowCount())
    {
        return QVariant();
    }

    // Select type.
    QSharedPointer<CustomType> type = this->project->types[index.row()];

    if (role == Qt::DisplayRole)
    {
        switch (index.column())
        {
            case 0:
                return type->name;

            case 1:
                if (type->isEnumeration())
                {
                    return "Enumeration";
                }
                else if (type->isList())
                {
                    return "List";
                }
                else
                {
                    return QVariant();
                }

            case 2:
                if (type->isEnumeration())
                {
                    return type->getEnumeration().join(", ");
                }
                else if (type->isList())
                {
                    return type->getItemType();
                }
                else
                {
                    return QVariant();
                }
        }

        return QVariant();
    }

    return QVariant();
}

QVariant CustomTypesTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal) {
            switch (section)
            {
                case 0:
                    return QString("Id");
                case 1:
                    return QString("Type");
                case 2:
                    return QString("Details");
            }
        }
    }
    return QVariant();
}

bool CustomTypesTableModel::insertRows(int position, int rows, const QModelIndex& index)
{
    Q_UNUSED(index);
    beginInsertRows(QModelIndex(), position, position + rows - 1);

    // Add new custom types.
    for (int row = 0; row < rows; ++row)
    {
        QSharedPointer<CustomType> type = QSharedPointer<CustomType>::create();
        this->project->types.insert(position, type);
    }

    endInsertRows();
    return true;
}

bool CustomTypesTableModel::removeRows(int position, int rows, const QModelIndex& index)
{
    Q_UNUSED(index);
    beginRemoveRows(QModelIndex(), position, position + rows - 1);

    // Remove field definitions.
    for (int row = 0; row < rows; ++row)
    {
        this->project->types.removeAt(position);
    }

    endRemoveRows();
    return true;
}

void CustomTypesTableModel::addEnumeration(const QString& name, const QStringList& enumeration)
{
    int index = this->rowCount();

    // Insert row at the end.
    this->insertRows(index, 1, QModelIndex());

    // Update type.
    this->updateEnumeration(index, name, enumeration);
}

void CustomTypesTableModel::addList(const QString& name, const QString& itemType)
{
    int index = this->rowCount();

    // Insert row at the end.
    this->insertRows(index, 1, QModelIndex());

    // Update type.
    this->updateList(index, name, itemType);
}

void CustomTypesTableModel::updateEnumeration(const int index, const QString& name, const QStringList& enumeration)
{
    // Get custom type.
    QSharedPointer<CustomType> type =  this->project->types.at(index);

    // Set data.
    type->name = name;
    type->baseType = BuiltInType::String;
    type->setEnumeration(enumeration);

    // Sort by display name.
    std::sort(this->project->types.begin(), this->project->types.end(), lessThanCustomTypes);

    // Update view.
    QModelIndex first = this->index(0, 0, QModelIndex());
    QModelIndex last = this->index(this->rowCount() - 1, 3, QModelIndex());

    emit(dataChanged(first, last));
}

void CustomTypesTableModel::updateList(const int index, const QString& name, const QString& itemType)
{
    // Get custom type.
    QSharedPointer<CustomType> type =  this->project->types.at(index);

    // Set data.
    type->name = name;
    type->baseType = BuiltInType::None;
    type->setItemType(itemType);

    // Sort by display name.
    std::sort(this->project->types.begin(), this->project->types.end(), lessThanCustomTypes);

    // Update view.
    QModelIndex first = this->index(0, 0, QModelIndex());
    QModelIndex last = this->index(this->rowCount() - 1, 3, QModelIndex());

    emit(dataChanged(first, last));
}

void CustomTypesTableModel::removeCustomType(const int index)
{
    // Remove row.
    this->removeRows(index, 1, QModelIndex());
}
