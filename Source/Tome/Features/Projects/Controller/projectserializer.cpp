#include "projectserializer.h"

#include <QXmlStreamWriter>

#include "../Model/project.h"
#include "../../../IO/xmlreader.h"

using namespace Tome;


const QString ProjectSerializer::AttributeBaseType = "BaseType";
const QString ProjectSerializer::AttributeExportedType = "ExportedType";
const QString ProjectSerializer::AttributeExportAsTable = "ExportAsTable";
const QString ProjectSerializer::AttributeExportRoots = "ExportRoots";
const QString ProjectSerializer::AttributeExportInnerNodes = "ExportInnerNodes";
const QString ProjectSerializer::AttributeExportLeafs = "ExportLeafs";
const QString ProjectSerializer::AttributeKey = "Key";
const QString ProjectSerializer::AttributePath = "Path";
const QString ProjectSerializer::AttributeTomeType = "TomeType";
const QString ProjectSerializer::AttributeValue = "Value";
const QString ProjectSerializer::AttributeVersion = "Version";
const QString ProjectSerializer::ElementComponents = "Components";
const QString ProjectSerializer::ElementFieldDefinitions = "FieldDefinitions";
const QString ProjectSerializer::ElementFileExtension = "FileExtension";
const QString ProjectSerializer::ElementMapping = "Mapping";
const QString ProjectSerializer::ElementName = "Name";
const QString ProjectSerializer::ElementLocale = "Locale";
const QString ProjectSerializer::ElementPath = "Path";
const QString ProjectSerializer::ElementRestriction = "Restriction";
const QString ProjectSerializer::ElementRestrictions = "Restrictions";
const QString ProjectSerializer::ElementRecords = "Records";
const QString ProjectSerializer::ElementRecordExportTemplates = "RecordExportTemplates";
const QString ProjectSerializer::ElementTemplate = "Template";
const QString ProjectSerializer::ElementTomeProject = "TomeProject";
const QString ProjectSerializer::ElementType = "Type";
const QString ProjectSerializer::ElementTypes = "Types";
const QString ProjectSerializer::ElementTypeMap = "TypeMap";

const int ProjectSerializer::Version = 3;


ProjectSerializer::ProjectSerializer()
{
}

void ProjectSerializer::serialize(QIODevice& device, QSharedPointer<Project> project) const
{
    // Open device stream.
    QXmlStreamWriter writer(&device);
    writer.setAutoFormatting(true);

    // Begin document.
    writer.writeStartDocument();
    {
        // Begin project.
        writer.writeStartElement(ElementTomeProject);
        {
            // Write version.
            writer.writeAttribute(AttributeVersion, QString::number(Version));

            // Write project name.
            writer.writeTextElement(ElementName, project->name);

            // Write project locale.
            writer.writeTextElement(ElementLocale, project->locale.name());

            // Write components.
            writer.writeStartElement(ElementComponents);
            {
                for (ComponentList::iterator it = project->components.begin();
                     it != project->components.end();
                     ++it)
                {
                    Component component = *it;
                    writer.writeTextElement(ElementName, component);
                }
            }
            writer.writeEndElement();

            // Write field definition set paths.
            writer.writeStartElement(ElementFieldDefinitions);
            {
                for (int i = 0; i < project->fieldDefinitionSets.size(); ++i)
                {
                    const FieldDefinitionSet& fieldDefinitionSet = project->fieldDefinitionSets[i];
                    writer.writeTextElement(ElementPath, fieldDefinitionSet.name);
                }
            }
            writer.writeEndElement();

            // Write record set paths.
            writer.writeStartElement(ElementRecords);
            {
                for (int i = 0; i < project->recordSets.size(); ++i)
                {
                    const RecordSet& recordSet = project->recordSets[i];
                    writer.writeTextElement(ElementPath, recordSet.name);
                }
            }
            writer.writeEndElement();

            // Write record export templates.
            writer.writeStartElement(ElementRecordExportTemplates);
            {
                for (RecordExportTemplateMap::const_iterator it = project->recordExportTemplates.begin();
                     it != project->recordExportTemplates.end();
                     ++it)
                {
                    writer.writeStartElement(ElementTemplate);
                    {
                        const RecordExportTemplate& exportTemplate = it.value();

                        if (exportTemplate.exportAsTable)
                        {
                            writer.writeAttribute(AttributeExportAsTable, "true");
                        }
                        if (!exportTemplate.path.isEmpty())
                        {
                            writer.writeAttribute(AttributePath, exportTemplate.path);
                        }

                        if (exportTemplate.exportRoots)
                        {
                            writer.writeAttribute(AttributeExportRoots, "true");
                        }

                        if (exportTemplate.exportInnerNodes)
                        {
                            writer.writeAttribute(AttributeExportInnerNodes, "true");
                        }

                        if (exportTemplate.exportLeafs)
                        {
                            writer.writeAttribute(AttributeExportLeafs, "true");
                        }

                        writer.writeTextElement(ElementName, exportTemplate.name);
                        writer.writeTextElement(ElementFileExtension, exportTemplate.fileExtension);

                        // Write export type map.
                        writer.writeStartElement(ElementTypeMap);
                        {
                            for (QMap<QString, QString>::const_iterator itTypeMap = exportTemplate.typeMap.begin();
                                 itTypeMap != exportTemplate.typeMap.end();
                                 ++itTypeMap)
                            {
                                writer.writeStartElement(ElementMapping);
                                writer.writeAttribute(AttributeTomeType, itTypeMap.key());
                                writer.writeAttribute(AttributeExportedType, itTypeMap.value());
                                writer.writeEndElement();
                            }
                        }
                        writer.writeEndElement();
                    }
                    writer.writeEndElement();
                }
            }
            writer.writeEndElement();

            // Write types.
            writer.writeStartElement(ElementTypes);
            {
                for (int i = 0; i < project->types.size(); ++i)
                {
                    const CustomType& type = project->types.at(i);

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
        // End project.
        writer.writeEndElement();
    }
    // End document.
    writer.writeEndDocument();
}

void ProjectSerializer::deserialize(QIODevice& device, QSharedPointer<Project> project) const
{
    // Open device stream.
    QXmlStreamReader streamReader(&device);
    XmlReader reader(streamReader);

    // Begin document.
    reader.readStartDocument();
    {
        // Read version.
        int version = reader.readAttribute(AttributeVersion).toInt();

        // Begin project.
        reader.readStartElement(ElementTomeProject);
        {
            // Read project name.
            project->name = reader.readTextElement(ElementName);

            // Read project locale.
            if (version > 1)
            {
                project->locale = QLocale(reader.readTextElement(ElementLocale));
            }

            // Read components.
            reader.readStartElement(ElementComponents);
            {
                while (reader.isAtElement(ElementName))
                {
                    const QString component = reader.readTextElement(ElementName);
                    project->components.push_back(component);
                }
            }
            reader.readEndElement();

            // Read field definition set paths.
            reader.readStartElement(ElementFieldDefinitions);
            {
                while (reader.isAtElement(ElementPath))
                {
                    FieldDefinitionSet fieldDefinitionSet = FieldDefinitionSet();
                    fieldDefinitionSet.name = reader.readTextElement(ElementPath);;
                    project->fieldDefinitionSets.push_back(fieldDefinitionSet);
                }
            }
            reader.readEndElement();

            // Read record set paths.
            reader.readStartElement(ElementRecords);
            {
                while (reader.isAtElement(ElementPath))
                {
                    RecordSet recordSet = RecordSet();
                    recordSet.name = reader.readTextElement(ElementPath);
                    project->recordSets.push_back(recordSet);
                }
            }
            reader.readEndElement();

            // Read record export templates.
            reader.readStartElement(ElementRecordExportTemplates);
            {
                while (reader.isAtElement(ElementTemplate))
                {
                    bool exportAsTable = reader.readAttribute(AttributeExportAsTable) == "true";
                    bool exportRoots = reader.readAttribute(AttributeExportRoots) == "true";
                    bool exportInnerNodes = reader.readAttribute(AttributeExportInnerNodes) == "true";
                    bool exportLeafs = reader.readAttribute(AttributeExportLeafs) == "true";
                    QString templatePath = reader.readAttribute(AttributePath);

                    reader.readStartElement(ElementTemplate);
                    {
                        RecordExportTemplate exportTemplate = RecordExportTemplate();

                        exportTemplate.exportAsTable = exportAsTable;
                        exportTemplate.exportRoots = exportRoots;
                        exportTemplate.exportInnerNodes = exportInnerNodes;
                        exportTemplate.exportLeafs = exportLeafs;

                        exportTemplate.path = templatePath;
                        exportTemplate.name = reader.readTextElement(ElementName);
                        exportTemplate.fileExtension = reader.readTextElement(ElementFileExtension);

                        // Read export type map.
                        reader.readStartElement(ElementTypeMap);
                        {
                            while (reader.isAtElement(ElementMapping))
                            {
                                QString typeMapKey = reader.readAttribute(AttributeTomeType);
                                QString typeMapValue = reader.readAttribute(AttributeExportedType);

                                exportTemplate.typeMap.insert(typeMapKey, typeMapValue);

                                // Advance reader.
                                reader.readEmptyElement(ElementMapping);
                            }
                        }
                        reader.readEndElement();

                        project->recordExportTemplates.insert(exportTemplate.name, exportTemplate);
                    }
                    reader.readEndElement();
                }
            }
            reader.readEndElement();

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

                    project->types.push_back(type);
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
