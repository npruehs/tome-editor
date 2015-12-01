#include "projectserializer.h"

#include <QXmlStreamWriter>

using namespace Tome;

ProjectSerializer::ProjectSerializer()
{
}

ProjectSerializer::serialize(QSharedPointer<QIODevice> device, QSharedPointer<Project> project)
{
    // Open device stream.
    QXmlStreamWriter stream(device.data());
    stream.setAutoFormatting(true);

    // Begin document.
    stream.writeStartDocument();

        // Begin project.
        stream.writeStartElement("TomeProject");

            // Write project name.
            stream.writeTextElement("Name", project->name);

            // Write field definition set paths.
            stream.writeStartElement("FieldDefinitions");
                for (std::list<QSharedPointer<Tome::FieldDefinitionSet> >::iterator it = project->fieldDefinitionSets.begin();
                     it != project->fieldDefinitionSets.end();
                     ++it)
                {
                    Tome::FieldDefinitionSet* itSet = it->data();
                    stream.writeTextElement("Path", itSet->name);
                }
            stream.writeEndElement();

            // Write record set paths.
            stream.writeStartElement("Records");
                for (std::list<QSharedPointer<Tome::RecordSet> >::iterator it = project->recordSets.begin();
                     it != project->recordSets.end();
                     ++it)
                {
                    Tome::RecordSet* itSet = it->data();
                    stream.writeTextElement("Path", itSet->name);
                }
            stream.writeEndElement();

        // End project.
        stream.writeEndElement();

    // End document.
    stream.writeEndDocument();
}
