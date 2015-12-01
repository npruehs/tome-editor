#include "recordsetserializer.h"

#include <QXmlStreamWriter>

using namespace Tome;

RecordSetSerializer::RecordSetSerializer()
{

}

void RecordSetSerializer::serialize(QSharedPointer<QIODevice> device, QSharedPointer<RecordSet> recordSet)
{
    // Open device stream.
    QXmlStreamWriter stream(device.data());
    stream.setAutoFormatting(true);

    // Begin document.
    stream.writeStartDocument();

        // Begin records.
        stream.writeStartElement("Records");

        // End records.
        stream.writeEndElement();

    // End document.
    stream.writeEndDocument();
}
