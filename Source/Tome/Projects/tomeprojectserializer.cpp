#include "tomeprojectserializer.h"

#include <QXmlStreamWriter>

using namespace Tome;

TomeProjectSerializer::TomeProjectSerializer()
{
}

TomeProjectSerializer::serialize(QSharedPointer<QIODevice> device, QSharedPointer<TomeProject> project)
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

            // Write field definition file paths.
            stream.writeStartElement("FieldDefinitions");
                for (std::list<QSharedPointer<Tome::FieldDefinitionFile> >::iterator it = project->fieldDefinitionFiles.begin();
                     it != project->fieldDefinitionFiles.end();
                     ++it)
                {
                    Tome::FieldDefinitionFile* itFile = it->data();
                    stream.writeTextElement("Path", itFile->path);
                }
            stream.writeEndElement();

        // End project.
        stream.writeEndElement();

    // End document.
    stream.writeEndDocument();
}
