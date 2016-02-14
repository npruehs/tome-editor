#include "recordsetserializer.h"

#include <QXmlStreamWriter>

#include "../../../IO/xmlreader.h"

using namespace Tome;


const QString RecordSetSerializer::ElementDisplayName = "DisplayName";
const QString RecordSetSerializer::ElementId = "Id";
const QString RecordSetSerializer::ElementRecord = "Record";
const QString RecordSetSerializer::ElementRecords = "Records";
const QString RecordSetSerializer::ElementValue = "Value";


RecordSetSerializer::RecordSetSerializer()
{

}

void RecordSetSerializer::serialize(QIODevice& device, const RecordSet& recordSet) const
{
    // Open device stream.
    QXmlStreamWriter stream(&device);
    stream.setAutoFormatting(true);

    // Begin document.
    stream.writeStartDocument();
    {
        // Begin records.
        stream.writeStartElement(ElementRecords);
        {
            // Write records.
            for (int i = 0; i < recordSet.records.size(); ++i)
            {
                const Record& record = recordSet.records[i];

                // Begin record.
                stream.writeStartElement(ElementRecord);
                {
                    // Write record.
                    stream.writeAttribute(ElementId, record.id);
                    stream.writeAttribute(ElementDisplayName, record.displayName);

                    for (QMap<QString, QVariant>::const_iterator it = record.fieldValues.begin();
                         it != record.fieldValues.end();
                         ++it)
                    {
                        QVariant value = it.value();

                        if (value.canConvert<QVariantList>())
                        {
                            QVariantList list = value.toList();

                            for (int i = 0; i < list.size(); ++i)
                            {
                                stream.writeStartElement(it.key());
                                stream.writeAttribute(ElementValue, list[i].toString());
                                stream.writeEndElement();
                            }
                        }
                        else
                        {
                            stream.writeStartElement(it.key());
                            stream.writeAttribute(ElementValue, it.value().toString());
                            stream.writeEndElement();
                        }
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


void RecordSetSerializer::deserialize(QIODevice& device, RecordSet& recordSet) const
{
    // Open device stream.
    QXmlStreamReader stream(&device);
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
                Record record = Record();

                // Read record.
                record.id = reader.readAttribute(ElementId);
                record.displayName = reader.readAttribute(ElementDisplayName);

                reader.readStartElement(ElementRecord);

                QString lastKey;
                QVariant lastValue;

                while (!reader.isAtElement(ElementRecord))
                {
                    QString key = reader.getElementName();
                    QVariant value = reader.readAttribute(ElementValue);

                    if (key == lastKey)
                    {
                        // List value.
                        if (lastValue.canConvert<QVariantList>())
                        {
                            QVariantList list = lastValue.toList();
                            list.append(value);
                            value = list;
                        }
                        else
                        {
                            QVariantList list = QVariantList();
                            list.append(lastValue);
                            list.append(value);
                            value = list;
                        }
                    }

                    record.fieldValues[key] = value;

                    reader.readEmptyElement(key);

                    lastKey = key;
                    lastValue = value;
                }

                recordSet.records.push_back(record);

                reader.readEndElement();
            }
        }
        // End records.
        reader.readEndElement();
    }
    // End document.
    reader.readEndDocument();
}
