#ifndef FIELDDEFINITIONSET_H
#define FIELDDEFINITIONSET_H

#include <QSharedPointer>
#include <QVector>

#include "fielddefinition.h"

namespace Tome
{
    class FieldDefinitionSet
    {
        public:
            FieldDefinitionSet();

            QString name;
            QVector< QSharedPointer<FieldDefinition> > fieldDefinitions;
    };
}

#endif // FIELDDEFINITIONSET_H
