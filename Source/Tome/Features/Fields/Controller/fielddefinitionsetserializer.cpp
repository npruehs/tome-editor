#include "fielddefinitionsetserializer.h"

#include <QXmlStreamWriter>

#include "../../../IO/xmlreader.h"

using namespace Tome;


const QString FieldDefinitionSetSerializer::AttributeComponent = "Component";
const QString FieldDefinitionSetSerializer::AttributeDefaultValue = "DefaultValue";
const QString FieldDefinitionSetSerializer::AttributeDescription = "Description";
const QString FieldDefinitionSetSerializer::AttributeDisplayName = "DisplayName";
const QString FieldDefinitionSetSerializer::AttributeId = "Id";
const QString FieldDefinitionSetSerializer::AttributeType = "Type";
const QString FieldDefinitionSetSerializer::AttributeValue = "Value";
const QString FieldDefinitionSetSerializer::ElementField = "Field";
const QString FieldDefinitionSetSerializer::ElementFields = "Fields";


FieldDefinitionSetSerializer::FieldDefinitionSetSerializer()
{

}

void FieldDefinitionSetSerializer::serialize(QIODevice& device, const FieldDefinitionSet& fieldDefinitionSet) const
{
    // Open device stream.
    QXmlStreamWriter stream(&device);
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
                stream.writeAttribute(AttributeType, fieldDefinition.fieldType);

                if (!fieldDefinition.component.isEmpty())
                {
                    stream.writeAttribute(AttributeComponent, fieldDefinition.component);
                }

                if (fieldDefinition.defaultValue.canConvert<QVariantList>())
                {
                    QVariantList list = fieldDefinition.defaultValue.toList();

                    for (int i = 0; i < list.size(); ++i)
                    {
                        stream.writeStartElement(AttributeDefaultValue);
                        stream.writeAttribute(AttributeValue, list[i].toString());
                        stream.writeEndElement();
                    }
                }
                else
                {
                    stream.writeAttribute(AttributeDefaultValue, fieldDefinition.defaultValue.toString());
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

void FieldDefinitionSetSerializer::deserialize(QIODevice& device, FieldDefinitionSet& fieldDefinitionSet) const
{
    // Open device stream.
    QXmlStreamReader stream(&device);
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

                reader.readStartElement(ElementField);

                // Check for list default value.
                QVariantList list;

                while (reader.isAtElement(AttributeDefaultValue))
                {
                    QVariant item = reader.readAttribute(AttributeValue);
                    list.append(item);
                    reader.readEmptyElement(AttributeDefaultValue);
                }

                if (!list.isEmpty())
                {
                    fieldDefinition.defaultValue = list;
                }

                reader.readEndElement();

                // Finish field definition.
                fieldDefinitionSet.fieldDefinitions.push_back(fieldDefinition);
            }
        }
        // End fields.
        reader.readEndElement();
    }
    // End document.
    reader.readEndDocument();
}
