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
const QString ProjectSerializer::AttributeIgnoreReadOnly = "IgnoreReadOnly";
const QString ProjectSerializer::AttributeKey = "Key";
const QString ProjectSerializer::AttributeRecordIdType = "RecordIdType";
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
const QString ProjectSerializer::ElementRecordImportTemplates = "RecordImportTemplates";
const QString ProjectSerializer::ElementTemplate = "Template";
const QString ProjectSerializer::ElementTomeProject = "TomeProject";
const QString ProjectSerializer::ElementType = "Type";
const QString ProjectSerializer::ElementTypes = "Types";
const QString ProjectSerializer::ElementTypeMap = "TypeMap";

const int ProjectSerializer::Version = 6;


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

            // Write record id type.
            writer.writeAttribute(AttributeRecordIdType, RecordIdType::toString(project->recordIdType));

            // Write lock behaviour.
            if (project->ignoreReadOnly)
            {
                writer.writeAttribute(AttributeIgnoreReadOnly, "true");
            }

            // Write project name.
            writer.writeTextElement(ElementName, project->name);

            // Write project locale.
            writer.writeTextElement(ElementLocale, project->locale.name());

            // Write components.
            writer.writeStartElement(ElementComponents);
            {
                for (int i = 0; i < project->componentSets.size(); ++i)
                {
                    const ComponentSet& componentSet = project->componentSets[i];
                    writer.writeTextElement(ElementPath, componentSet.name);
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

            // Write record export template paths.
            writer.writeStartElement(ElementRecordExportTemplates);
            {
                for (RecordExportTemplateList::const_iterator it = project->recordExportTemplates.cbegin();
                     it != project->recordExportTemplates.cend();
                     ++it)
                {
                    const RecordExportTemplate& exportTemplate = *it;
                    writer.writeTextElement(ElementPath, exportTemplate.path);
                }
            }
            writer.writeEndElement();

            // Write type set paths.
            writer.writeStartElement(ElementTypes);
            {
                for (int i = 0; i < project->typeSets.size(); ++i)
                {
                    const CustomTypeSet& typeSet = project->typeSets[i];
                    writer.writeTextElement(ElementPath, typeSet.name);
                }
            }
            writer.writeEndElement();

            // Write record import template paths.
            writer.writeStartElement(ElementRecordImportTemplates);
            {
                for (RecordTableImportTemplateList::const_iterator it = project->recordTableImportTemplates.cbegin();
                     it != project->recordTableImportTemplates.cend();
                     ++it)
                {
                    const RecordTableImportTemplate& importTemplate = *it;
                    writer.writeTextElement(ElementPath, importTemplate.path);
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
    XmlReader reader(&device);

    // Validate project file.
    reader.validate(":/Source/Tome/Features/Projects/Model/TomeProject6.xsd",
                    QObject::tr("Invalid project file: %1 (line %2, column %3)"));

    // Begin document.
    reader.readStartDocument();
    {
        // Read record id type.
        project->recordIdType = RecordIdType::fromString(reader.readAttribute(AttributeRecordIdType));

        if (project->recordIdType == RecordIdType::Invalid)
        {
            project->recordIdType = RecordIdType::String;
        }

        // Read lock behaviour.
        project->ignoreReadOnly = reader.readAttribute(AttributeIgnoreReadOnly) == "true";

        // Begin project.
        reader.readStartElement(ElementTomeProject);
        {
            // Read project name.
            project->name = reader.readTextElement(ElementName);

            // Read project locale.
            project->locale = QLocale(reader.readTextElement(ElementLocale));

            // Read components.
            reader.readStartElement(ElementComponents);
            {
                while (reader.isAtElement(ElementPath))
                {
                    ComponentSet componentSet = ComponentSet();
                    componentSet.name = reader.readTextElement(ElementPath);
                    project->componentSets.push_back(componentSet);
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
                while (reader.isAtElement(ElementPath))
                {
                    RecordExportTemplate exportTemplate = RecordExportTemplate();
                    exportTemplate.path = reader.readTextElement(ElementPath);
                    project->recordExportTemplates << exportTemplate;
                }
            }
            reader.readEndElement();

            // Read types.
            reader.readStartElement(ElementTypes);
            {
                while (reader.isAtElement(ElementPath))
                {
                    CustomTypeSet typeSet = CustomTypeSet();
                    typeSet.name = reader.readTextElement(ElementPath);
                    project->typeSets.push_back(typeSet);
                }
            }
            reader.readEndElement();

            // Read record export templates.
            reader.readStartElement(ElementRecordImportTemplates);
            {
                while (reader.isAtElement(ElementPath))
                {
                    RecordTableImportTemplate importTemplate = RecordTableImportTemplate();
                    importTemplate.path = reader.readTextElement(ElementPath);
                    project->recordTableImportTemplates << importTemplate;
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
