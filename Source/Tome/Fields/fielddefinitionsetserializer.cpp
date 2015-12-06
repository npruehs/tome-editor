#include "fielddefinitionsetserializer.h"

#include <QXmlStreamWriter>

#include "../Values/valueconverter.h"

using namespace Tome;

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

        // Begin fields.
        stream.writeStartElement("Fields");

        // Write fields.
        for (std::list<QSharedPointer<FieldDefinition> >::iterator it = fieldDefinitionSet->fieldDefinitions.begin();
             it != fieldDefinitionSet->fieldDefinitions.end();
             ++it)
        {
            FieldDefinition* fieldDefinition = it->data();

            stream.writeStartElement("Field");
            stream.writeAttribute("Id", fieldDefinition->id);
            stream.writeAttribute("DisplayName", fieldDefinition->displayName);
            stream.writeAttribute("Description", fieldDefinition->description);
            stream.writeAttribute("DefaultValue", fieldDefinition->defaultValue);
            stream.writeAttribute("Type", valueConverter->FieldTypeToString(fieldDefinition->fieldType));
            stream.writeEndElement();
        }

        // End fields.
        stream.writeEndElement();

    // End document.
    stream.writeEndDocument();
}
