#ifndef INTFIELDDEFINITION_H
#define INTFIELDDEFINITION_H

#include "fielddefinition.h"

namespace Tome
{
    class IntFieldDefinition : public FieldDefinition
    {
    public:
        FieldType::FieldType getFieldType();
    };
}

#endif // INTFIELDDEFINITION_H
