#ifndef FIELDDEFINITION_H
#define FIELDDEFINITION_H

#include <QString>

#include "fieldtype.h"

namespace Tome
{
    class FieldDefinition
    {
    public:
        virtual const FieldType::FieldType getFieldType() const = 0;

        QString description;
        QString displayName;
        QString id;
    };
}

#endif // FIELDDEFINITION_H
