#include "recordsetserializer.h"

#include <QXmlStreamWriter>

#include "../../../IO/xmlreader.h"

using namespace Tome;


const QString RecordSetSerializer::ElementDisplayName = "DisplayName";
const QString RecordSetSerializer::ElementId = "Id";
const QString RecordSetSerializer::ElementItem = "Item";
const QString RecordSetSerializer::ElementParentId = "Parent";
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

                    if (!record.parentId.isEmpty())
                    {
                        stream.writeAttribute(ElementParentId, record.parentId);
                    }

                    for (QMap<QString, QVariant>::const_iterator it = record.fieldValues.begin();
                         it != record.fieldValues.end();
                         ++it)
                    {
                        QVariant value = it.value();

                        if (value.canConvert<QVariantList>())
                        {
                            QVariantList list = value.toList();

                            stream.writeStartElement(it.key());

                            for (int i = 0; i < list.size(); ++i)
                            {
                                stream.writeStartElement(ElementItem);
                                stream.writeAttribute(ElementValue, list[i].toString());
                                stream.writeEndElement();
                            }

                            stream.writeEndElement();
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
                record.parentId = reader.readAttribute(ElementParentId);

                reader.readStartElement(ElementRecord);

                while (!reader.isAtElement(ElementRecord))
                {
                    QString key = reader.getElementName();
                    QVariant value = reader.readAttribute(ElementValue);

                    if (value.toString().isEmpty())
                    {
                        reader.readStartElement(key);
                        {
                            // Begin list.
                            QVariantList list = QVariantList();

                            while (reader.isAtElement(ElementItem))
                            {
                                // Read list item.
                                QVariant item = reader.readAttribute(ElementValue);
                                list.append(item);
                                reader.readEmptyElement(ElementItem);
                            }

                            value = list;
                        }
                        reader.readEndElement();
                    }
                    else
                    {
                        reader.readEmptyElement(key);
                    }

                    record.fieldValues[key] = value;
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
