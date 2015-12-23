#ifndef PROJECT_H
#define PROJECT_H

#include <QSharedPointer>
#include <QString>
#include <QVector>

#include "../Fields/fielddefinitionset.h"
#include "../Records/recordset.h"

namespace Tome
{
    class Project
    {
        public:
            Project();

            QString name;
            QString path;
            QVector< QSharedPointer<FieldDefinitionSet> > fieldDefinitionSets;
            QVector< QSharedPointer<RecordSet> > recordSets;

            /**
             * @brief getRecordByDisplayName Returns the record with the specified display name, or a null pointer if that record could not be found.
             * @param displayName Display name of the record to get.
             * @return Record with the specified display name, or a null pointer if that record could not be found.
             */
            QSharedPointer<Record> getRecordByDisplayName(const QString& displayName);
    };
}

#endif // PROJECT_H
