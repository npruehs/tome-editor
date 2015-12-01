#include "fielddefinitionsetserializer.h"

#include <QXmlStreamWriter>

using namespace Tome;

FieldDefinitionSetSerializer::FieldDefinitionSetSerializer()
{

}

void FieldDefinitionSetSerializer::serialize(QSharedPointer<QIODevice> device, QSharedPointer<FieldDefinitionSet> fieldDefinitionSet)
{
    // Open device stream.
    QXmlStreamWriter stream(device.data());
    stream.setAutoFormatting(true);

    // Begin document.
    stream.writeStartDocument();

        // Begin fields.
        stream.writeStartElement("Fields");

        // End fields.
        stream.writeEndElement();

    // End document.
    stream.writeEndDocument();
}
