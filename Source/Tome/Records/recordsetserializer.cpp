#include "recordsetserializer.h"

#include <QXmlStreamWriter>

#include "../IO/xmlreader.h"

using namespace Tome;


const QString RecordSetSerializer::ElementId = "Id";
const QString RecordSetSerializer::ElementRecord = "Record";
const QString RecordSetSerializer::ElementRecords = "Records";
const QString RecordSetSerializer::ElementValue = "Value";


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
        stream.writeStartElement(ElementRecords);
        {
            // Write records.
            for (QVector<QSharedPointer<Record> >::iterator it = recordSet->records.begin();
                 it != recordSet->records.end();
                 ++it)
            {
                Record* record = it->data();

                // Begin record.
                stream.writeStartElement(ElementRecord);
                {
                    // Write record.
                    stream.writeAttribute(ElementId, record->id);

                    for (std::map<QString, QString>::iterator it = record->fieldValues.begin();
                         it != record->fieldValues.end();
                         ++it)
                    {
                        stream.writeStartElement(it->first);
                        stream.writeAttribute(ElementValue, it->second);
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


void RecordSetSerializer::deserialize(QSharedPointer<QIODevice> device, QSharedPointer<RecordSet> recordSet) const
{
    // Open device stream.
    QSharedPointer<QXmlStreamReader> stream =
            QSharedPointer<QXmlStreamReader>::create(device.data());
    XmlReader reader(stream);

    // Begin document.
    reader.readStartDocument();
    {
        // Begin records.
        reader.readStartElement(ElementRecords);
        {
            // Read records.
            while (reader.isAtElement(ElementRecord))
            {
                // Add new record.
                QSharedPointer<Record> record = QSharedPointer<Record>::create();
                recordSet->records.push_back(record);

                // Read record.
                record->id = reader.readAttribute(ElementId);

                reader.readStartElement(ElementRecord);

                while (!reader.isAtElement(ElementRecord))
                {
                    const QString key = reader.getElementName();
                    const QString value = reader.readAttribute(ElementValue);

                    record->fieldValues[key] = value;

                    reader.readEmptyElement(key);
                }

                reader.readEndElement();
            }
        }
        // End records.
        reader.readEndElement();
    }
    // End document.
    reader.readEndDocument();
}
