#ifndef FIELDDEFINITIONSETSERIALIZER_H
#define FIELDDEFINITIONSETSERIALIZER_H

#include <QIODevice>

namespace Tome
{
    class FieldDefinitionSet;

    class FieldDefinitionSetSerializer
    {
        public:
            FieldDefinitionSetSerializer();

            /**
             * @brief serialize Writes the passed field definition set to the specified device.
             * @param device Device to write the field definition set to.
             * @param fieldDefinitionSet Field definition set to write.
             */
            void serialize(QIODevice& device, const FieldDefinitionSet& fieldDefinitionSet) const;

            /**
             * @brief serialize Reads the passed field definition set from the specified device.
             * @param device Device to read the field definition set from.
             * @param fieldDefinitionSet Field definition set to fill.
             */
            void deserialize(QIODevice& device, FieldDefinitionSet& fieldDefinitionSet) const;

        private:
            static const QString AttributeComponent;
            static const QString AttributeDefaultValue;
            static const QString AttributeDescription;
            static const QString AttributeDisplayName;
            static const QString AttributeId;
            static const QString AttributeKey;
            static const QString AttributeType;
            static const QString AttributeValue;

            static const QString ElementField;
            static const QString ElementFields;

    };
}

#endif // FIELDDEFINITIONSETSERIALIZER_H
