#include "projectserializer.h"

#include <QXmlStreamWriter>

#include "../IO/xmlreader.h"

using namespace Tome;


const QString ProjectSerializer::ElementFieldDefinitions = "FieldDefinitions";
const QString ProjectSerializer::ElementFileExtension = "FileExtension";
const QString ProjectSerializer::ElementName = "Name";
const QString ProjectSerializer::ElementPath = "Path";
const QString ProjectSerializer::ElementRecords = "Records";
const QString ProjectSerializer::ElementRecordExportTemplates = "RecordExportTemplates";
const QString ProjectSerializer::ElementTemplate = "Template";
const QString ProjectSerializer::ElementTomeProject = "TomeProject";

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
        writer.writeStartElement(ElementTomeProject);
        {
            // Write project name.
            writer.writeTextElement(ElementName, project->name);

            // Write field definition set paths.
            writer.writeStartElement(ElementFieldDefinitions);
            {
                for (QVector<QSharedPointer<Tome::FieldDefinitionSet> >::iterator it = project->fieldDefinitionSets.begin();
                     it != project->fieldDefinitionSets.end();
                     ++it)
                {
                    Tome::FieldDefinitionSet* itSet = it->data();
                    writer.writeTextElement(ElementPath, itSet->name);
                }
            }
            writer.writeEndElement();

            // Write record set paths.
            writer.writeStartElement(ElementRecords);
            {
                for (QVector<QSharedPointer<Tome::RecordSet> >::iterator it = project->recordSets.begin();
                     it != project->recordSets.end();
                     ++it)
                {
                    Tome::RecordSet* itSet = it->data();
                    writer.writeTextElement(ElementPath, itSet->name);
                }
            }
            writer.writeEndElement();

            // Write record export templates.
            writer.writeStartElement(ElementRecordExportTemplates);
            {
                for (QMap<QString, QSharedPointer<RecordExportTemplate> >::iterator it = project->recordExportTemplates.begin();
                     it != project->recordExportTemplates.end();
                     ++it)
                {
                    writer.writeStartElement(ElementTemplate);
                    {
                        QSharedPointer<RecordExportTemplate> exportTemplate = it.value();

                        writer.writeTextElement(ElementName, exportTemplate->name);
                        writer.writeTextElement(ElementFileExtension, exportTemplate->fileExtension);
                    }
                    writer.writeEndElement();
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
        reader.readStartElement(ElementTomeProject);
        {
            // Read project name.
            project->name = reader.readTextElement(ElementName);

            // Read field definition set paths.
            reader.readStartElement(ElementFieldDefinitions);
            {
                while (reader.isAtElement(ElementPath))
                {
                    const QString name = reader.readTextElement(ElementPath);
                    QSharedPointer<FieldDefinitionSet> fieldDefinitionSet =
                            QSharedPointer<FieldDefinitionSet>::create();
                    fieldDefinitionSet->name = name;
                    project->fieldDefinitionSets.push_back(fieldDefinitionSet);
                }
            }
            reader.readEndElement();

            // Read record set paths.
            reader.readStartElement(ElementRecords);
            {
                while (reader.isAtElement(ElementPath))
                {
                    const QString name = reader.readTextElement(ElementPath);
                    QSharedPointer<RecordSet> recordSet =
                            QSharedPointer<RecordSet>::create();
                    recordSet->name = name;
                    project->recordSets.push_back(recordSet);
                }
            }
            reader.readEndElement();

            // Read record export templates.
            reader.readStartElement(ElementRecordExportTemplates);
            {
                while (reader.isAtElement(ElementTemplate))
                {
                    reader.readStartElement(ElementTemplate);
                    {
                        QSharedPointer<RecordExportTemplate> exportTemplate =
                                QSharedPointer<RecordExportTemplate>::create();

                        exportTemplate->name = reader.readTextElement(ElementName);
                        exportTemplate->fileExtension = reader.readTextElement(ElementFileExtension);

                        project->recordExportTemplates.insert(exportTemplate->name, exportTemplate);
                    }
                    reader.readEndElement();
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
