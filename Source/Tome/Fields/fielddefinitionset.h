#ifndef FIELDDEFINITIONSET_H
#define FIELDDEFINITIONSET_H

#include <list>
#include <QSharedPointer>

#include "fielddefinition.h"

namespace Tome
{
    class FieldDefinitionSet
    {
    public:
        FieldDefinitionSet();

        QString name;
        std::list< QSharedPointer<FieldDefinition> > fieldDefinitions;
    };
}

#endif // FIELDDEFINITIONSET_H
