#include "project.h"

using namespace Tome;

Project::Project()
{
}

QSharedPointer<FieldDefinition> Project::getFieldDefinition(const QString& id) const
{
    for (QVector<QSharedPointer<FieldDefinitionSet> >::const_iterator itFieldDefinitionSets = this->fieldDefinitionSets.begin();
         itFieldDefinitionSets != this->fieldDefinitionSets.end();
         ++itFieldDefinitionSets)
    {
        QSharedPointer<FieldDefinitionSet> fieldDefinitionSet = *itFieldDefinitionSets;

        for (QVector<QSharedPointer<FieldDefinition> >::const_iterator itFieldDefinitions = fieldDefinitionSet->fieldDefinitions.begin();
             itFieldDefinitions != fieldDefinitionSet->fieldDefinitions.end();
             ++itFieldDefinitions)
        {
            QSharedPointer<FieldDefinition> fieldDefinition = *itFieldDefinitions;

            if (fieldDefinition->id == id)
            {
                return fieldDefinition;
            }
        }
    }

    return QSharedPointer<FieldDefinition>(0);
}

QSharedPointer<Record> Project::getRecord(const QString& id) const
{
    for (QVector<QSharedPointer<RecordSet> >::const_iterator itRecordSets = this->recordSets.begin();
         itRecordSets != this->recordSets.end();
         ++itRecordSets)
    {
        QSharedPointer<RecordSet> recordSet = *itRecordSets;

        for (QVector<QSharedPointer<Record> >::const_iterator itRecords = recordSet->records.begin();
             itRecords != recordSet->records.end();
             ++itRecords)
        {
            QSharedPointer<Record> record = *itRecords;

            if (record->id == id)
            {
                return record;
            }
        }
    }

    return QSharedPointer<Record>(0);
}

QSharedPointer<Record> Project::getRecordByDisplayName(const QString& displayName) const
{
    for (QVector<QSharedPointer<RecordSet> >::const_iterator itRecordSets = this->recordSets.begin();
         itRecordSets != this->recordSets.end();
         ++itRecordSets)
    {
        QSharedPointer<RecordSet> recordSet = *itRecordSets;

        for (QVector<QSharedPointer<Record> >::const_iterator itRecords = recordSet->records.begin();
             itRecords != recordSet->records.end();
             ++itRecords)
        {
            QSharedPointer<Record> record = *itRecords;

            if (record->displayName == displayName)
            {
                return record;
            }
        }
    }

    return QSharedPointer<Record>(0);
}

QStringList Project::getRecordNames() const
{
    QStringList names;

    for (QVector<QSharedPointer<RecordSet> >::const_iterator itRecordSets = this->recordSets.begin();
         itRecordSets != this->recordSets.end();
         ++itRecordSets)
    {
        QSharedPointer<RecordSet> recordSet = *itRecordSets;

        for (QVector<QSharedPointer<Record> >::const_iterator itRecords = recordSet->records.begin();
             itRecords != recordSet->records.end();
             ++itRecords)
        {
            QSharedPointer<Record> record = *itRecords;
            names << record->displayName;
        }
    }

    return names;
}
