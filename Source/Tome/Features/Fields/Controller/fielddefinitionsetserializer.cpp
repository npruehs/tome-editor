#include "fielddefinitionsetserializer.h"

#include <QXmlStreamWriter>

#include "../Model/fielddefinitionset.h"
#include "../../../IO/xmlreader.h"

using namespace Tome;


const QString FieldDefinitionSetSerializer::AttributeComponent = "Component";
const QString FieldDefinitionSetSerializer::AttributeDefaultValue = "DefaultValue";
const QString FieldDefinitionSetSerializer::AttributeDescription = "Description";
const QString FieldDefinitionSetSerializer::AttributeDisplayName = "DisplayName";
const QString FieldDefinitionSetSerializer::AttributeId = "Id";
const QString FieldDefinitionSetSerializer::AttributeKey = "Key";
const QString FieldDefinitionSetSerializer::AttributeType = "Type";
const QString FieldDefinitionSetSerializer::AttributeValue = "Value";
const QString FieldDefinitionSetSerializer::ElementFacet = "Facet";
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
                else if (fieldDefinition.defaultValue.canConvert<QVariantMap>())
                {
                    QVariantMap map = fieldDefinition.defaultValue.toMap();

                    for (QVariantMap::iterator it = map.begin(); it != map.end(); ++it)
                    {
                        stream.writeStartElement(AttributeDefaultValue);
                        stream.writeAttribute(AttributeKey, it.key());
                        stream.writeAttribute(AttributeValue, it.value().toString());
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
    XmlReader reader(&device);

    // Validate fields file.
    reader.validate(":/Source/Tome/Features/Fields/Model/TomeFields.xsd",
                    QObject::tr("Invalid fields file: %1 (line %2, column %3)"));

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
                fieldDefinition.fieldDefinitionSetName = fieldDefinitionSet.name;

                QString component = reader.readAttribute(AttributeComponent);

                if (!component.isEmpty())
                {
                    fieldDefinition.component = component;
                }

                reader.readStartElement(ElementField);

                // Check for list default value.
                QVariantList list;
                QVariantMap map;

                while (reader.isAtElement(AttributeDefaultValue))
                {
                    QString key = reader.readAttribute(AttributeKey);
                    QVariant value = reader.readAttribute(AttributeValue);

                    if (!key.isEmpty())
                    {
                        map[key] = value;
                    }
                    else
                    {
                        list.append(value);
                    }

                    reader.readEmptyElement(AttributeDefaultValue);
                }

                if (!map.isEmpty())
                {
                    fieldDefinition.defaultValue = map;
                }
                else if (!list.isEmpty())
                {
                    fieldDefinition.defaultValue = list;
                }

                // Read facets.
                // TODO(np): Remove in next major release.
                while (reader.isAtElement(ElementFacet))
                {
                    reader.readEmptyElement(ElementFacet);
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
