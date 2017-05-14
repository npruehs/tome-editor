#ifndef FIELDDEFINITIONSET_H
#define FIELDDEFINITIONSET_H

#include <QVector>

#include "fielddefinitionlist.h"

namespace Tome
{
    class FieldDefinitionSet
    {
        public:
            QString name;
            FieldDefinitionList fieldDefinitions;
    };
}

#endif // FIELDDEFINITIONSET_H
