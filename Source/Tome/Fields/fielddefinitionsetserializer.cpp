#include "fielddefinitionsetserializer.h"

#include <QXmlStreamWriter>

#include "../IO/xmlreader.h"
#include "../Values/valueconverter.h"

using namespace Tome;


const QString FieldDefinitionSetSerializer::ElementDefaultValue = "DefaultValue";
const QString FieldDefinitionSetSerializer::ElementDescription = "Description";
const QString FieldDefinitionSetSerializer::ElementDisplayName = "DisplayName";
const QString FieldDefinitionSetSerializer::ElementField = "Field";
const QString FieldDefinitionSetSerializer::ElementFields = "Fields";
const QString FieldDefinitionSetSerializer::ElementId = "Id";
const QString FieldDefinitionSetSerializer::ElementType = "Type";

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
            for (std::list<QSharedPointer<FieldDefinition> >::iterator it = fieldDefinitionSet->fieldDefinitions.begin();
                 it != fieldDefinitionSet->fieldDefinitions.end();
                 ++it)
            {
                FieldDefinition* fieldDefinition = it->data();

                stream.writeStartElement(ElementField);
                stream.writeAttribute(ElementId, fieldDefinition->id);
                stream.writeAttribute(ElementDisplayName, fieldDefinition->displayName);
                stream.writeAttribute(ElementDescription, fieldDefinition->description);
                stream.writeAttribute(ElementDefaultValue, fieldDefinition->defaultValue);
                stream.writeAttribute(ElementType, valueConverter->FieldTypeToString(fieldDefinition->fieldType));
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
                fieldDefinition->id = reader.readAttribute(ElementId);
                fieldDefinition->displayName = reader.readAttribute(ElementDisplayName);
                fieldDefinition->description = reader.readAttribute(ElementDescription);
                fieldDefinition->defaultValue = reader.readAttribute(ElementDefaultValue);
                fieldDefinition->fieldType = valueConverter->StringToFieldType(reader.readAttribute(ElementType));

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
