#include "project.h"

#include <stdexcept>

#include "../Types/builtintype.h"

using namespace Tome;


Project::Project()
{
}

QStringList Project::getComponentNames() const
{
    QStringList components;

    for (ComponentList::const_iterator itComponents = this->components.begin();
         itComponents != this->components.end();
         ++itComponents)
    {
        const Component& component = *itComponents;
        components << component;
    }

    return components;
}

QSharedPointer<CustomType> Project::getCustomType(const QString& name) const
{
    for (QVector<QSharedPointer<CustomType> >::const_iterator it = this->types.begin();
         it != this->types.end();
         ++it)
    {
        QSharedPointer<CustomType> type = *it;
        if (type->name == name)
        {
            return type;
        }
    }

    return QSharedPointer<CustomType>(0);
}

Record& Project::getRecord(const QString& id)
{
    for (int i = 0; i < this->recordSets.size(); ++i)
    {
        RecordSet& recordSet = this->recordSets[i];

        for (int j = 0; j < recordSet.records.size(); ++j)
        {
            Record& record = recordSet.records[j];

            if (record.id == id)
            {
                return record;
            }
        }
    }

    throw std::out_of_range(id.toStdString());
}

Record& Project::getRecordByDisplayName(const QString& displayName)
{
    for (int i = 0; i < this->recordSets.size(); ++i)
    {
        RecordSet& recordSet = this->recordSets[i];

        for (int j = 0; j < recordSet.records.size(); ++j)
        {
            Record& record = recordSet.records[j];

            if (record.displayName == displayName)
            {
                return record;
            }
        }
    }

    throw std::out_of_range(displayName.toStdString());
}

QStringList Project::getRecordNames() const
{
    QStringList names;

    for (int i = 0; i < this->recordSets.size(); ++i)
    {
        const RecordSet& recordSet = this->recordSets[i];

        for (int j = 0; j < recordSet.records.size(); ++j)
        {
            const Record& record = recordSet.records[j];
            names << record.displayName;
        }
    }

    return names;
}

QStringList Project::getTypeNames() const
{
    QStringList typeNames;
    typeNames.push_back(BuiltInType::Boolean);
    typeNames.push_back(BuiltInType::Color);
    typeNames.push_back(BuiltInType::Integer);
    typeNames.push_back(BuiltInType::Real);
    typeNames.push_back(BuiltInType::Reference);
    typeNames.push_back(BuiltInType::String);

    for (QVector<QSharedPointer<CustomType> >::const_iterator it = this->types.begin();
         it != this->types.end();
         ++it)
    {
        QSharedPointer<CustomType> type = *it;
        typeNames.push_back(type->name);
    }

    return typeNames;
}
