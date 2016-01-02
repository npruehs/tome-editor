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

            QString component;
            QString defaultValue;
            QString description;
            QString displayName;
            FieldType::FieldType fieldType;
            QString id;

    };
}

#endif // FIELDDEFINITION_H
