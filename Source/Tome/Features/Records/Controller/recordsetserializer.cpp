#include "recordsetserializer.h"

#include <QXmlStreamWriter>

#include "../Model/recordset.h"
#include "../../../IO/xmlreader.h"

using namespace Tome;


const QString RecordSetSerializer::ElementDisplayName = "DisplayName";
const QString RecordSetSerializer::ElementEditorIconFieldId = "EditorIconFieldId";
const QString RecordSetSerializer::ElementId = "Id";
const QString RecordSetSerializer::ElementItem = "Item";
const QString RecordSetSerializer::ElementKey = "Key";
const QString RecordSetSerializer::ElementParentId = "Parent";
const QString RecordSetSerializer::ElementReadOnly = "ReadOnly";
const QString RecordSetSerializer::ElementRecord = "Record";
const QString RecordSetSerializer::ElementRecords = "Records";
const QString RecordSetSerializer::ElementValue = "Value";


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

                // Report progress.
                emit progressChanged(tr("Saving Data"), record.id, i, recordSet.records.size());

                // Begin record.
                stream.writeStartElement(ElementRecord);
                {
                    // Write record.
                    stream.writeAttribute(ElementId, record.id);
                    stream.writeAttribute(ElementDisplayName, record.displayName);

                    if (record.readOnly)
                    {
                        stream.writeAttribute(ElementReadOnly, "true");
                    }

                    if (!record.parentId.isEmpty())
                    {
                        stream.writeAttribute(ElementParentId, record.parentId);
                    }

                    if (!record.editorIconFieldId.isEmpty())
                    {
                        stream.writeAttribute(ElementEditorIconFieldId, record.editorIconFieldId);
                    }

                    for (QMap<QString, QVariant>::const_iterator it = record.fieldValues.begin();
                         it != record.fieldValues.end();
                         ++it)
                    {
                        QVariant value = it.value();

                        // Write key.
                        stream.writeStartElement(it.key());

                        // Write value.
                        {
                            if (value.canConvert<QVariantList>())
                            {
                                QVariantList list = value.toList();

                                for (int i = 0; i < list.size(); ++i)
                                {
                                    stream.writeStartElement(ElementItem);
                                    stream.writeAttribute(ElementValue, list[i].toString());
                                    stream.writeEndElement();
                                }
                            }
                            else if (value.canConvert<QVariantMap>())
                            {
                                QVariantMap map = value.toMap();

                                for (QVariantMap::iterator it = map.begin(); it != map.end(); ++it)
                                {
                                    stream.writeStartElement(ElementItem);
                                    stream.writeAttribute(ElementKey, it.key());
                                    stream.writeAttribute(ElementValue, it.value().toString());
                                    stream.writeEndElement();
                                }
                            }
                            else
                            {
                                stream.writeAttribute(ElementValue, it.value().toString());
                            }
                        }

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

    // Report finish.
    emit progressChanged(tr("Saving Data"), QString(), 1, 1);
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
                record.editorIconFieldId = reader.readAttribute(ElementEditorIconFieldId);
                record.parentId = reader.readAttribute(ElementParentId);
                record.readOnly = reader.readAttribute(ElementReadOnly) == "true";
                record.recordSetName = recordSet.name;

                // Report progress.
                emit progressChanged(tr("Loading Data"), record.id, device.pos(), device.size());

                reader.readStartElement(ElementRecord);

                while (!reader.isAtElement(ElementRecord))
                {
                    QString key = reader.getElementName();
                    QVariant value = reader.readAttribute(ElementValue);

                    if (value.toString().isEmpty())
                    {
                        reader.readStartElement(key);
                        {
                            // Begin list or map.
                            QVariantList list;
                            QVariantMap map;

                            while (reader.isAtElement(ElementItem))
                            {
                                // Read item.
                                QString key = reader.readAttribute(ElementKey);
                                QVariant value = reader.readAttribute(ElementValue);

                                if (!key.isEmpty())
                                {
                                    map[key] = value;
                                }
                                else
                                {
                                    list.append(value);
                                }

                                reader.readEmptyElement(ElementItem);
                            }

                            if (!map.isEmpty())
                            {
                                value = map;
                            }
                            else if (!list.isEmpty())
                            {
                                value = list;
                            }
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

    // Report finish.
    emit progressChanged(tr("Loading Data"), QString(), 1, 1);
}
