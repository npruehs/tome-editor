#include "fielddefinitionfileserializer.h"

#include <QXmlStreamWriter>

using namespace Tome;

FieldDefinitionFileSerializer::FieldDefinitionFileSerializer()
{

}

void FieldDefinitionFileSerializer::serialize(QSharedPointer<QIODevice> device, QSharedPointer<FieldDefinitionFile> fieldDefinitionFile)
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
