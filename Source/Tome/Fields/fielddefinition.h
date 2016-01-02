#ifndef FIELDDEFINITION_H
#define FIELDDEFINITION_H

#include <QString>

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
            QString fieldType;
            QString id;

    };
}

#endif // FIELDDEFINITION_H
