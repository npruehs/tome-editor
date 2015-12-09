#include "projectserializer.h"

#include <QXmlStreamWriter>

#include "../IO/xmlreader.h"

using namespace Tome;

ProjectSerializer::ProjectSerializer()
{
}

void ProjectSerializer::serialize(QSharedPointer<QIODevice> device, QSharedPointer<Project> project) const
{
    // Open device stream.
    QXmlStreamWriter writer(device.data());
    writer.setAutoFormatting(true);

    // Begin document.
    writer.writeStartDocument();
    {
        // Begin project.
        writer.writeStartElement("TomeProject");
        {
            // Write project name.
            writer.writeTextElement("Name", project->name);

            // Write field definition set paths.
            writer.writeStartElement("FieldDefinitions");
            {
                for (std::list<QSharedPointer<Tome::FieldDefinitionSet> >::iterator it = project->fieldDefinitionSets.begin();
                     it != project->fieldDefinitionSets.end();
                     ++it)
                {
                    Tome::FieldDefinitionSet* itSet = it->data();
                    writer.writeTextElement("Path", itSet->name);
                }
            }
            writer.writeEndElement();

            // Write record set paths.
            writer.writeStartElement("Records");
            {
                for (std::list<QSharedPointer<Tome::RecordSet> >::iterator it = project->recordSets.begin();
                     it != project->recordSets.end();
                     ++it)
                {
                    Tome::RecordSet* itSet = it->data();
                    writer.writeTextElement("Path", itSet->name);
                }
            }
            writer.writeEndElement();
        }
        // End project.
        writer.writeEndElement();
    }
    // End document.
    writer.writeEndDocument();
}

void ProjectSerializer::deserialize(QSharedPointer<QIODevice> device, QSharedPointer<Project> project) const
{
    // Open device stream.
    QSharedPointer<QXmlStreamReader> streamReader =
            QSharedPointer<QXmlStreamReader>::create(device.data());
    XmlReader reader(streamReader);

    // Begin document.
    reader.readStartDocument();
    {
        // Begin project.
        reader.readStartElement("TomeProject");
        {
            // Read project name.
            project->name = reader.readTextElement("Name");

            // Read field definition set paths.
            reader.readStartElement("FieldDefinitions");
            {
                while (reader.isAtElement("Path"))
                {
                    const QString name = reader.readTextElement("Path");
                    QSharedPointer<FieldDefinitionSet> fieldDefinitionSet =
                            QSharedPointer<FieldDefinitionSet>::create();
                    fieldDefinitionSet->name = name;
                    project->fieldDefinitionSets.push_back(fieldDefinitionSet);
                }
            }
            reader.readEndElement();

            // Read record set paths.
            reader.readStartElement("Records");
            {
                while (reader.isAtElement("Path"))
                {
                    const QString name = reader.readTextElement("Path");
                    QSharedPointer<RecordSet> recordSet =
                            QSharedPointer<RecordSet>::create();
                    recordSet->name = name;
                    project->recordSets.push_back(recordSet);
                }
            }
            reader.readEndElement();
        }
        // End project.
        reader.readEndElement();
    }
    // End document.
    reader.readEndDocument();
}
