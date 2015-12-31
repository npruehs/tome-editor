#ifndef FIELDDEFINITIONSETSERIALIZER_H
#define FIELDDEFINITIONSETSERIALIZER_H

#include <QIODevice>
#include <QSharedPointer>

#include "fielddefinitionset.h"

namespace Tome
{
    class FieldDefinitionSetSerializer
    {
        public:
            FieldDefinitionSetSerializer();

            /**
             * @brief serialize Writes the passed field definition set to the specified device.
             * @param device Device to write the field definition set to.
             * @param fieldDefinitionSet Field definition set to write.
             */
            void serialize(QSharedPointer<QIODevice> device, QSharedPointer<FieldDefinitionSet> fieldDefinitionSet) const;

            /**
             * @brief serialize Reads the passed field definition set from the specified device.
             * @param device Device to read the field definition set from.
             * @param fieldDefinitionSet Field definition set to fill.
             */
            void deserialize(QSharedPointer<QIODevice> device, QSharedPointer<FieldDefinitionSet> fieldDefinitionSet) const;

        private:
            static const QString ElementDefaultValue;
            static const QString ElementDescription;
            static const QString ElementDisplayName;
            static const QString ElementField;
            static const QString ElementFields;
            static const QString ElementId;
            static const QString ElementType;
    };
}

#endif // FIELDDEFINITIONSETSERIALIZER_H
