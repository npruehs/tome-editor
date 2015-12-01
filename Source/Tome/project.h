#ifndef PROJECT_H
#define PROJECT_H

#include <list>

#include <QSharedPointer>
#include <QString>

#include "Fields/fielddefinitionset.h"

namespace Tome
{
    class Project
    {
    public:
        Project();

        QString name;
        std::list< QSharedPointer<FieldDefinitionSet> > fieldDefinitionSets;
    };
}

#endif // PROJECT_H
