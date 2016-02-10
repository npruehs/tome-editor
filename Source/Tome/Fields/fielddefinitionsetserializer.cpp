#include "fielddefinitionsetserializer.h"

#include <QXmlStreamWriter>

#include "../IO/xmlreader.h"

using namespace Tome;


const QString FieldDefinitionSetSerializer::AttributeComponent = "Component";
const QString FieldDefinitionSetSerializer::AttributeDefaultValue = "DefaultValue";
const QString FieldDefinitionSetSerializer::AttributeDescription = "Description";
const QString FieldDefinitionSetSerializer::AttributeDisplayName = "DisplayName";
const QString FieldDefinitionSetSerializer::AttributeId = "Id";
const QString FieldDefinitionSetSerializer::AttributeType = "Type";
const QString FieldDefinitionSetSerializer::ElementField = "Field";
const QString FieldDefinitionSetSerializer::ElementFields = "Fields";


FieldDefinitionSetSerializer::FieldDefinitionSetSerializer()
{

}

void FieldDefinitionSetSerializer::serialize(QSharedPointer<QIODevice> device, const FieldDefinitionSet& fieldDefinitionSet) const
{
    // Open device stream.
    QXmlStreamWriter stream(device.data());
    stream.setAutoFormatting(true);

    // Begin document.
    stream.writeStartDocument();
    {
        // Begin fields.
        stream.writeStartElement(ElementFields);
        {
            // Write fields.
            for (int i = 0; i < fieldDefinitionSet.fieldDefinitions.size(); ++i)
            {
                const FieldDefinition& fieldDefinition = fieldDefinitionSet.fieldDefinitions[i];

                stream.writeStartElement(ElementField);
                stream.writeAttribute(AttributeId, fieldDefinition.id);
                stream.writeAttribute(AttributeDisplayName, fieldDefinition.displayName);
                stream.writeAttribute(AttributeDescription, fieldDefinition.description);
                stream.writeAttribute(AttributeDefaultValue, fieldDefinition.defaultValue);
                stream.writeAttribute(AttributeType, fieldDefinition.fieldType);

                if (!fieldDefinition.component.isEmpty())
                {
                    stream.writeAttribute(AttributeComponent, fieldDefinition.component);
                }

                stream.writeEndElement();
            }
        }
        // End fields.
        stream.writeEndElement();
    }
    // End document.
    stream.writeEndDocument();
}

void FieldDefinitionSetSerializer::deserialize(QSharedPointer<QIODevice> device, FieldDefinitionSet& fieldDefinitionSet) const
{
    // Open device stream.
    QSharedPointer<QXmlStreamReader> stream =
            QSharedPointer<QXmlStreamReader>::create(device.data());
    XmlReader reader(stream);

    // Begin document.
    reader.readStartDocument();
    {
        // Begin fields.
        reader.readStartElement(ElementFields);
        {
            // Read fields.
            while (reader.isAtElement(ElementField))
            {
                // Add field definition.
                FieldDefinition fieldDefinition = FieldDefinition();

                // Read attribute values.
                fieldDefinition.id = reader.readAttribute(AttributeId);
                fieldDefinition.displayName = reader.readAttribute(AttributeDisplayName);
                fieldDefinition.description = reader.readAttribute(AttributeDescription);
                fieldDefinition.defaultValue = reader.readAttribute(AttributeDefaultValue);
                fieldDefinition.fieldType = reader.readAttribute(AttributeType);

                QString component = reader.readAttribute(AttributeComponent);

                if (!component.isEmpty())
                {
                    fieldDefinition.component = component;
                }

                fieldDefinitionSet.fieldDefinitions.push_back(fieldDefinition);

                // Advance reader.
                reader.readEmptyElement(ElementField);
            }
        }
        // End fields.
        reader.readEndElement();
    }
    // End document.
    reader.readEndDocument();
}
