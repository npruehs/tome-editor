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
    };
}

#endif // PROJECT_H
