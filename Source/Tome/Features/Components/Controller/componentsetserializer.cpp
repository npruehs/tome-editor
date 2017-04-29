#include "componentsetserializer.h"

#include <QXmlStreamWriter>

#include "../Model/componentset.h"
#include "../../../IO/xmlreader.h"

using namespace Tome;


const QString ComponentSetSerializer::ElementComponents = "Components";
const QString ComponentSetSerializer::ElementName = "Name";


ComponentSetSerializer::ComponentSetSerializer()
{
}

void ComponentSetSerializer::serialize(QIODevice& device, const Tome::ComponentSet& componentSet) const
{
    // Open device stream.
    QXmlStreamWriter writer(&device);
    writer.setAutoFormatting(true);

    // Begin document.
    writer.writeStartDocument();
    {
        // Write components.
        writer.writeStartElement(ElementComponents);
        {
            for (ComponentList::const_iterator it = componentSet.components.cbegin();
                 it != componentSet.components.cend();
                 ++it)
            {
                Component component = *it;
                writer.writeTextElement(ElementName, component);
            }
        }
        writer.writeEndElement();
    }
    // End document.
    writer.writeEndDocument();
}

void ComponentSetSerializer::deserialize(QIODevice& device, ComponentSet& componentSet) const
{
    // Open device stream.
    XmlReader reader(&device);

    // Begin document.
    reader.readStartDocument();
    {
        // Read components.
        reader.readStartElement(ElementComponents);
        {
            while (reader.isAtElement(ElementName))
            {
                const QString component = reader.readTextElement(ElementName);
                componentSet.components.push_back(component);
            }
        }
        reader.readEndElement();
    }
    // End document.
    reader.readEndDocument();
}
