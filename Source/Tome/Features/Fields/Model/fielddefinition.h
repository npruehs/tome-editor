#ifndef FIELDDEFINITION_H
#define FIELDDEFINITION_H

#include <QString>
#include <QVariant>

#include "../../Components/Model/component.h"


namespace Tome
{
    class FieldDefinition
    {
        public:
            Component component;
            QVariant defaultValue;
            QString description;
            QString displayName;
            QString fieldDefinitionSetName;
            QString fieldType;
            QString id;
    };

    inline bool operator==(const FieldDefinition& lhs, const FieldDefinition& rhs){ return lhs.id == rhs.id; }
    inline bool operator!=(const FieldDefinition& lhs, const FieldDefinition& rhs){ return !(lhs == rhs); }

    inline bool fieldDefinitionLessThanDisplayName(const FieldDefinition& e1, const FieldDefinition& e2)
    {
        return e1.displayName.toLower() < e2.displayName.toLower();
    }

    inline bool fieldDefinitionLessThanComponentAndDisplayName(const FieldDefinition& e1, const FieldDefinition& e2)
    {
        QString lhs = QString("%1%2").arg(e1.component, e1.displayName);
        QString rhs = QString("%1%2").arg(e2.component, e2.displayName);
        return lhs.toLower() < rhs.toLower();
    }
}

#endif // FIELDDEFINITION_H
