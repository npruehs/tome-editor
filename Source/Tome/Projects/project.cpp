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
