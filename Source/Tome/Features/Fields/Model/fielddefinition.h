#ifndef FIELDDEFINITION_H
#define FIELDDEFINITION_H

#include <QString>
#include <QVariant>

#include "../../Components/Model/component.h"


namespace Tome
{
    /**
     * @brief Field that can be added to a record to hold data.
     */
    class FieldDefinition
    {
        public:
            /**
             * @brief Component this field definition belongs to.
             */
            Component component;

            /**
             * @brief Default value of this field.
             */
            QVariant defaultValue;

            /**
             * @brief Textual description of this field.
             */
            QString description;

            /**
             * @brief Display name of this field.
             */
            QString displayName;

            /**
             * @brief Name of the set this field definition belongs to.
             */
            QString fieldDefinitionSetName;

            /**
             * @brief Type of the values of this field.
             */
            QString fieldType;

            /**
             * @brief Unique id of this field definition.
             */
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
