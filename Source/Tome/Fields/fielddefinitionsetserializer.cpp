#include "fielddefinitionsetserializer.h"

#include <QXmlStreamWriter>

#include "../IO/xmlreader.h"
#include "../Values/valueconverter.h"

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

void FieldDefinitionSetSerializer::serialize(QSharedPointer<QIODevice> device, QSharedPointer<FieldDefinitionSet> fieldDefinitionSet) const
{
    // Setup value conversion.
    QSharedPointer<ValueConverter> valueConverter = QSharedPointer<ValueConverter>::create();

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
            for (QVector<QSharedPointer<FieldDefinition> >::iterator it = fieldDefinitionSet->fieldDefinitions.begin();
                 it != fieldDefinitionSet->fieldDefinitions.end();
                 ++it)
            {
                FieldDefinition* fieldDefinition = it->data();

                stream.writeStartElement(ElementField);
                stream.writeAttribute(AttributeId, fieldDefinition->id);
                stream.writeAttribute(AttributeDisplayName, fieldDefinition->displayName);
                stream.writeAttribute(AttributeDescription, fieldDefinition->description);
                stream.writeAttribute(AttributeDefaultValue, fieldDefinition->defaultValue);
                stream.writeAttribute(AttributeType, fieldDefinition->fieldType);

                if (!fieldDefinition->component.isEmpty())
                {
                    stream.writeAttribute(AttributeComponent, fieldDefinition->component);
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

void FieldDefinitionSetSerializer::deserialize(QSharedPointer<QIODevice> device, QSharedPointer<FieldDefinitionSet> fieldDefinitionSet) const
{
    // Setup value conversion.
    QSharedPointer<ValueConverter> valueConverter = QSharedPointer<ValueConverter>::create();

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
                QSharedPointer<FieldDefinition> fieldDefinition =
                        QSharedPointer<FieldDefinition>::create();

                fieldDefinitionSet->fieldDefinitions.push_back(fieldDefinition);

                // Read attribute values.
                fieldDefinition->id = reader.readAttribute(AttributeId);
                fieldDefinition->displayName = reader.readAttribute(AttributeDisplayName);
                fieldDefinition->description = reader.readAttribute(AttributeDescription);
                fieldDefinition->defaultValue = reader.readAttribute(AttributeDefaultValue);
                fieldDefinition->fieldType = reader.readAttribute(AttributeType);

                QString component = reader.readAttribute(AttributeComponent);

                if (!component.isEmpty())
                {
                    fieldDefinition->component = component;
                }

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
