#include "customtypesetserializer.h"

#include <QXmlStreamWriter>

#include "../Model/customtypeset.h"
#include "../../../IO/xmlreader.h"

using namespace Tome;


const QString CustomTypeSetSerializer::AttributeKey = "Key";
const QString CustomTypeSetSerializer::AttributeValue = "Value";
const QString CustomTypeSetSerializer::ElementName = "Name";
const QString CustomTypeSetSerializer::ElementRestriction = "Restriction";
const QString CustomTypeSetSerializer::ElementRestrictions = "Restrictions";
const QString CustomTypeSetSerializer::ElementType = "Type";
const QString CustomTypeSetSerializer::ElementTypes = "Types";


CustomTypeSetSerializer::CustomTypeSetSerializer()
{
}

void CustomTypeSetSerializer::serialize(QIODevice& device, const CustomTypeSet& customTypeSet) const
{
    // Open device stream.
    QXmlStreamWriter writer(&device);
    writer.setAutoFormatting(true);

    // Begin document.
    writer.writeStartDocument();
    {
        // Write types.
        writer.writeStartElement(ElementTypes);
        {
            for (int i = 0; i < customTypeSet.types.size(); ++i)
            {
                const CustomType& type = customTypeSet.types.at(i);

                writer.writeStartElement(ElementType);
                {
                    writer.writeAttribute(ElementName, type.name);

                    // Write restrictions map.
                    writer.writeStartElement(ElementRestrictions);
                    {
                        for (QMap<QString, QString>::const_iterator itRestrictions = type.restrictions.begin();
                             itRestrictions != type.restrictions.end();
                             ++itRestrictions)
                        {
                            writer.writeStartElement(ElementRestriction);
                            writer.writeAttribute(AttributeKey, itRestrictions.key());
                            writer.writeAttribute(AttributeValue, itRestrictions.value());
                            writer.writeEndElement();
                        }
                    }
                    writer.writeEndElement();
                }
                writer.writeEndElement();
            }
        }
        writer.writeEndElement();
    }
    // End document.
    writer.writeEndDocument();
}

void CustomTypeSetSerializer::deserialize(QIODevice& device, CustomTypeSet& customTypeSet) const
{
    // Open device stream.
    QXmlStreamReader streamReader(&device);
    XmlReader reader(streamReader);

    // Begin document.
    reader.readStartDocument();
    {
        // Read types.
        reader.readStartElement(ElementTypes);
        {
            while (reader.isAtElement(ElementType))
            {
                CustomType type = CustomType();

                type.name = reader.readAttribute(ElementName);

                reader.readStartElement(ElementType);
                {
                    // Read type restriction map.
                    reader.readStartElement(ElementRestrictions);
                    {
                        while (reader.isAtElement(ElementRestriction))
                        {
                            QString restrictionKey = reader.readAttribute(AttributeKey);
                            QString restrictionValue = reader.readAttribute(AttributeValue);

                            type.restrictions.insert(restrictionKey, restrictionValue);

                            // Advance reader.
                            reader.readEmptyElement(ElementRestriction);
                        }
                    }
                    reader.readEndElement();
                }
                reader.readEndElement();

                customTypeSet.types.push_back(type);
            }
        }
        reader.readEndElement();
    }
    // End document.
    reader.readEndDocument();
}
