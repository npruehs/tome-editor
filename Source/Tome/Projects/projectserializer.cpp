#include "projectserializer.h"

#include <QXmlStreamWriter>

#include "../IO/xmlreader.h"

using namespace Tome;


const QString ProjectSerializer::AttributeBaseType = "BaseType";
const QString ProjectSerializer::AttributeExportedType = "ExportedType";
const QString ProjectSerializer::AttributeKey = "Key";
const QString ProjectSerializer::AttributeTomeType = "TomeType";
const QString ProjectSerializer::AttributeValue = "Value";
const QString ProjectSerializer::ElementComponents = "Components";
const QString ProjectSerializer::ElementFieldDefinitions = "FieldDefinitions";
const QString ProjectSerializer::ElementFileExtension = "FileExtension";
const QString ProjectSerializer::ElementMapping = "Mapping";
const QString ProjectSerializer::ElementName = "Name";
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

            // Write components.
            writer.writeStartElement(ElementComponents);
            {
                for (QVector<QString>::iterator it = project->components.begin();
                     it != project->components.end();
                     ++it)
                {
                    QString component = *it;
                    writer.writeTextElement(ElementName, component);
                }
            }
            writer.writeEndElement();

            // Write field definition set paths.
            writer.writeStartElement(ElementFieldDefinitions);
            {
                for (QVector<QSharedPointer<Tome::FieldDefinitionSet> >::iterator it = project->fieldDefinitionSets.begin();
                     it != project->fieldDefinitionSets.end();
                     ++it)
                {
                    Tome::FieldDefinitionSet* fieldDefinitionSet = it->data();
                    writer.writeTextElement(ElementPath, fieldDefinitionSet->name);
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
                    Tome::RecordSet* recordSet = it->data();
                    writer.writeTextElement(ElementPath, recordSet->name);
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

                        // Write export type map.
                        writer.writeStartElement(ElementTypeMap);
                        {
                            for (QMap<QString, QString>::iterator itTypeMap = exportTemplate->typeMap.begin();
                                 itTypeMap != exportTemplate->typeMap.end();
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
                for (QVector<QSharedPointer<CustomFieldType> >::iterator it = project->types.begin();
                     it != project->types.end();
                     ++it)
                {
                    QSharedPointer<CustomFieldType> type = *it;

                    writer.writeStartElement(ElementType);
                    {
                        writer.writeAttribute(ElementName, type->name);

                        if (!type->baseType.isEmpty())
                        {
                            writer.writeAttribute(AttributeBaseType, type->baseType);
                        }

                        // Write restrictions map.
                        writer.writeStartElement(ElementRestrictions);
                        {
                            for (QMap<QString, QString>::iterator itRestrictions = type->restrictions.begin();
                                 itRestrictions != type->restrictions.end();
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

                        // Read export type map.
                        reader.readStartElement(ElementTypeMap);
                        {
                            while (reader.isAtElement(ElementMapping))
                            {
                                QString typeMapKey = reader.readAttribute(AttributeTomeType);
                                QString typeMapValue = reader.readAttribute(AttributeExportedType);

                                exportTemplate->typeMap.insert(typeMapKey, typeMapValue);

                                // Advance reader.
                                reader.readEmptyElement(ElementMapping);
                            }
                        }
                        reader.readEndElement();

                        project->recordExportTemplates.insert(exportTemplate->name, exportTemplate);
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
                    reader.readStartElement(ElementType);
                    {
                        QSharedPointer<CustomFieldType> type =
                                QSharedPointer<CustomFieldType>::create();

                        type->name = reader.readAttribute(ElementName);
                        type->baseType = reader.readAttribute(AttributeBaseType);

                        // Read type restriction map.
                        reader.readStartElement(ElementRestrictions);
                        {
                            while (reader.isAtElement(ElementRestriction))
                            {
                                QString restrictionKey = reader.readAttribute(AttributeKey);
                                QString restrictionValue = reader.readAttribute(AttributeValue);

                                type->restrictions.insert(restrictionKey, restrictionValue);

                                // Advance reader.
                                reader.readEmptyElement(ElementRestriction);
                            }
                        }
                        reader.readEndElement();

                        project->types.push_back(type);
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
