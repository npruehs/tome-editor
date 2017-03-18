#include "customtypesetserializer.h"

#include <QXmlStreamWriter>

#include "../Model/customtypeset.h"
#include "../../../IO/xmlreader.h"

using namespace Tome;


const QString CustomTypeSetSerializer::AttributeKey = "Key";
const QString CustomTypeSetSerializer::AttributeValue = "Value";
const QString CustomTypeSetSerializer::AttributeVersionDeprecated = "Value";
const QString CustomTypeSetSerializer::AttributeVersion = "Version";
const QString CustomTypeSetSerializer::ElementConstrainingFacet = "ConstrainingFacet";
const QString CustomTypeSetSerializer::ElementConstrainingFacets = "ConstrainingFacets";
const QString CustomTypeSetSerializer::ElementFundamentalFacet = "FundamentalFacet";
const QString CustomTypeSetSerializer::ElementFundamentalFacets = "FundamentalFacets";
const QString CustomTypeSetSerializer::ElementName = "Name";
const QString CustomTypeSetSerializer::ElementRestriction = "Restriction";
const QString CustomTypeSetSerializer::ElementRestrictions = "Restrictions";
const QString CustomTypeSetSerializer::ElementType = "Type";
const QString CustomTypeSetSerializer::ElementTypes = "Types";

const int CustomTypeSetSerializer::Version = 2;


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
            // Write version.
            writer.writeAttribute(AttributeVersion, QString::number(Version));

            for (int i = 0; i < customTypeSet.types.size(); ++i)
            {
                const CustomType& type = customTypeSet.types.at(i);

                writer.writeStartElement(ElementType);
                {
                    writer.writeAttribute(ElementName, type.name);

                    // Write facet maps.
                    writer.writeStartElement(ElementFundamentalFacets);
                    {
                        for (QVariantMap::const_iterator itFundamentalFacets = type.fundamentalFacets.cbegin();
                             itFundamentalFacets != type.fundamentalFacets.cend();
                             ++itFundamentalFacets)
                        {
                            writer.writeStartElement(ElementFundamentalFacet);
                            writer.writeAttribute(AttributeKey, itFundamentalFacets.key());
                            writer.writeAttribute(AttributeValue, itFundamentalFacets.value().toString());
                            writer.writeEndElement();
                        }
                    }
                    writer.writeEndElement();

                    writer.writeStartElement(ElementConstrainingFacets);
                    {
                        for (QVariantMap::const_iterator itConstrainingFacets = type.constrainingFacets.cbegin();
                             itConstrainingFacets != type.constrainingFacets.cend();
                             ++itConstrainingFacets)
                        {
                            writer.writeStartElement(ElementConstrainingFacet);
                            writer.writeAttribute(AttributeKey, itConstrainingFacets.key());
                            writer.writeAttribute(AttributeValue, itConstrainingFacets.value().toString());
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
        // Read version.
        int version = reader.readAttribute(AttributeVersion).toInt();

        if (version == 0)
        {
            version = reader.readAttribute(AttributeVersionDeprecated).toInt();
        }

        // Read types.
        reader.readStartElement(ElementTypes);
        {
            while (reader.isAtElement(ElementType))
            {
                CustomType type = CustomType();

                type.name = reader.readAttribute(ElementName);
                type.typeSetName = customTypeSet.name;

                reader.readStartElement(ElementType);
                {
                    if (version >= 2)
                    {
                        // Read facet maps.
                        reader.readStartElement(ElementFundamentalFacets);
                        {
                            while (reader.isAtElement(ElementFundamentalFacet))
                            {
                                QString restrictionKey = reader.readAttribute(AttributeKey);
                                QString restrictionValue = reader.readAttribute(AttributeValue);

                                type.fundamentalFacets.insert(restrictionKey, restrictionValue);

                                // Advance reader.
                                reader.readEmptyElement(ElementFundamentalFacet);
                            }
                        }
                        reader.readEndElement();

                        reader.readStartElement(ElementConstrainingFacets);
                        {
                            while (reader.isAtElement(ElementConstrainingFacet))
                            {
                                QString restrictionKey = reader.readAttribute(AttributeKey);
                                QString restrictionValue = reader.readAttribute(AttributeValue);

                                type.constrainingFacets.insert(restrictionKey, restrictionValue);

                                // Advance reader.
                                reader.readEmptyElement(ElementConstrainingFacet);
                            }
                        }
                        reader.readEndElement();
                    }
                    else
                    {
                        // Read type restriction map.
                        reader.readStartElement(ElementRestrictions);
                        {
                            while (reader.isAtElement(ElementRestriction))
                            {
                                QString restrictionKey = reader.readAttribute(AttributeKey);
                                QString restrictionValue = reader.readAttribute(AttributeValue);

                                type.fundamentalFacets.insert(restrictionKey, restrictionValue);

                                // Advance reader.
                                reader.readEmptyElement(ElementRestriction);
                            }
                        }
                        reader.readEndElement();
                    }
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
