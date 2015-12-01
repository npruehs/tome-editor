#ifndef FIELDDEFINITION_H
#define FIELDDEFINITION_H

#include <QString>

#include "fieldtype.h"

namespace Tome
{
    class FieldDefinition
    {
    public:
        FieldDefinition();

        FieldType::FieldType fieldType;
        QString description;
        QString displayName;
        QString id;
        QString defaultValue;
    };
}

#endif // FIELDDEFINITION_H
