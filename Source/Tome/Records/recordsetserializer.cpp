#include "recordsetserializer.h"

#include <QXmlStreamWriter>

using namespace Tome;

RecordSetSerializer::RecordSetSerializer()
{

}

void RecordSetSerializer::serialize(QSharedPointer<QIODevice> device, QSharedPointer<RecordSet> recordSet) const
{
    // Open device stream.
    QXmlStreamWriter stream(device.data());
    stream.setAutoFormatting(true);

    // Begin document.
    stream.writeStartDocument();
    {
        // Begin records.
        stream.writeStartElement("Records");
        {
            // Write records.
            for (std::list<QSharedPointer<Record> >::iterator it = recordSet->records.begin();
                 it != recordSet->records.end();
                 ++it)
            {
                Record* record = it->data();

                // Begin record.
                stream.writeStartElement("Record");
                {
                    // Write record.
                    stream.writeAttribute("Id", record->id);

                    for (std::map<QString, QString>::iterator it = record->fieldValues.begin();
                         it != record->fieldValues.end();
                         ++it)
                    {
                        stream.writeStartElement(it->first);
                        stream.writeAttribute("Value", it->second);
                        stream.writeEndElement();
                    }
                }
                // End record.
                stream.writeEndElement();
            }
        }
        // End records.
        stream.writeEndElement();
    }
    // End document.
    stream.writeEndDocument();
}
