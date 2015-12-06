#ifndef PROJECT_H
#define PROJECT_H

#include <list>

#include <QSharedPointer>
#include <QString>

#include "../Fields/fielddefinitionset.h"
#include "../Records/recordset.h"

namespace Tome
{
    class Project
    {
        public:
            Project();

            QString name;
            std::list< QSharedPointer<FieldDefinitionSet> > fieldDefinitionSets;
            std::list< QSharedPointer<RecordSet> > recordSets;
    };
}

#endif // PROJECT_H
