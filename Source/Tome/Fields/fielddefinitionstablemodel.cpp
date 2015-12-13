#include "fielddefinitionstablemodel.h"

#include "../Values/valueconverter.h"

using namespace Tome;


FieldDefinitionsTableModel::FieldDefinitionsTableModel(QSharedPointer<Tome::Project> project)
    : QAbstractTableModel(0),
      project(project)
{
}

int FieldDefinitionsTableModel::rowCount(const QModelIndex & /*parent*/) const
{
   return this->project->fieldDefinitionSets.size();
}

int FieldDefinitionsTableModel::columnCount(const QModelIndex & /*parent*/) const
{
    return 5;
}

QVariant FieldDefinitionsTableModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        // Select field definition.
        QSharedPointer<FieldDefinitionSet> fieldDefinitionSet = this->project->fieldDefinitionSets[0];
        QSharedPointer<FieldDefinition> fieldDefinition = fieldDefinitionSet->fieldDefinitions[index.row()];

        // Show data.
        QSharedPointer<ValueConverter> valueConverter = QSharedPointer<ValueConverter>::create();

        switch (index.column())
        {
            case 0:
                return fieldDefinition->id;
            case 1:
                return fieldDefinition->displayName;
            case 2:
                return valueConverter->FieldTypeToString(fieldDefinition->fieldType);
            case 3:
                return fieldDefinition->defaultValue;
            case 4:
                return fieldDefinition->description;
        }

        return QVariant();
    }

    return QVariant();
}

QVariant FieldDefinitionsTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal) {
            switch (section)
            {
            case 0:
                return QString("Id");
            case 1:
                return QString("Display Name");
            case 2:
                return QString("Type");
            case 3:
                return QString("Default Value");
            case 4:
                return QString("Description");
            }
        }
    }
    return QVariant();
}
