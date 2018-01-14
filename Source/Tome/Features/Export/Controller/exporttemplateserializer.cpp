#include "exporttemplateserializer.h"

#include "QXmlStreamWriter"

#include "../Model/recordexporttemplate.h"
#include "../../../IO/xmlreader.h"

using namespace Tome;


const QString ExportTemplateSerializer::AttributeExportAsTable = "ExportAsTable";
const QString ExportTemplateSerializer::AttributeExportRoots = "ExportRoots";
const QString ExportTemplateSerializer::AttributeExportInnerNodes = "ExportInnerNodes";
const QString ExportTemplateSerializer::AttributeExportLeafs = "ExportLeafs";
const QString ExportTemplateSerializer::AttributeExportLocalizedFieldsOnly = "ExportLocalizedFieldsOnly";
const QString ExportTemplateSerializer::AttributeExportedType = "ExportedType";
const QString ExportTemplateSerializer::AttributeTomeType = "TomeType";
const QString ExportTemplateSerializer::AttributeVersion = "Version";
const QString ExportTemplateSerializer::ElementFileExtension = "FileExtension";
const QString ExportTemplateSerializer::ElementId = "Id";
const QString ExportTemplateSerializer::ElementIgnoredFields = "IgnoredFields";
const QString ExportTemplateSerializer::ElementIgnoredRecords = "IgnoredRecords";
const QString ExportTemplateSerializer::ElementMapping = "Mapping";
const QString ExportTemplateSerializer::ElementName = "Name";
const QString ExportTemplateSerializer::ElementReplaceWith = "ReplaceWith";
const QString ExportTemplateSerializer::ElementString = "String";
const QString ExportTemplateSerializer::ElementStringReplacementMap = "StringReplacementMap";
const QString ExportTemplateSerializer::ElementTemplate = "Template";
const QString ExportTemplateSerializer::ElementTypeMap = "TypeMap";

const int ExportTemplateSerializer::Version = 2;


ExportTemplateSerializer::ExportTemplateSerializer()
{
}

void ExportTemplateSerializer::serialize(QIODevice& device, const RecordExportTemplate& exportTemplate) const
{
    // Open device stream.
    QXmlStreamWriter writer(&device);
    writer.setAutoFormatting(true);

    // Begin document.
    writer.writeStartDocument();
    {
        writer.writeStartElement(ElementTemplate);
        {
            // Write version.
            writer.writeAttribute(AttributeVersion, QString::number(Version));

            // Write export flags.
            if (exportTemplate.exportAsTable)
            {
                writer.writeAttribute(AttributeExportAsTable, "true");
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

            if (exportTemplate.exportLocalizedFieldsOnly)
            {
                writer.writeAttribute(AttributeExportLocalizedFieldsOnly, "true");
            }

            // Write name and file extension.
            writer.writeTextElement(ElementName, exportTemplate.name);
            writer.writeTextElement(ElementFileExtension, exportTemplate.fileExtension);

            // Write export type map.
            writer.writeStartElement(ElementTypeMap);
            {
                for (QMap<QString, QString>::const_iterator itTypeMap = exportTemplate.typeMap.cbegin();
                     itTypeMap != exportTemplate.typeMap.cend();
                     ++itTypeMap)
                {
                    writer.writeStartElement(ElementMapping);
                    writer.writeAttribute(AttributeTomeType, itTypeMap.key());
                    writer.writeAttribute(AttributeExportedType, itTypeMap.value());
                    writer.writeEndElement();
                }
            }
            writer.writeEndElement();

            // Write string replacement map.
            writer.writeStartElement(ElementStringReplacementMap);
            {
                for (auto itStringReplacementMap = exportTemplate.stringReplacementMap.cbegin();
                     itStringReplacementMap != exportTemplate.stringReplacementMap.cend();
                     ++itStringReplacementMap)
                {
                    writer.writeStartElement(ElementMapping);
                    writer.writeTextElement(ElementString, itStringReplacementMap.key());
                    writer.writeTextElement(ElementReplaceWith, itStringReplacementMap.value());
                    writer.writeEndElement();
                }
            }
            writer.writeEndElement();

            // Write ignore lists.
            writer.writeStartElement(ElementIgnoredRecords);
            {
                for (int i = 0; i < exportTemplate.ignoredRecords.size(); ++i)
                {
                    writer.writeTextElement(ElementId, exportTemplate.ignoredRecords[i]);
                }
            }
            writer.writeEndElement();

            writer.writeStartElement(ElementIgnoredFields);
            {
                for (int i = 0; i < exportTemplate.ignoredFields.size(); ++i)
                {
                    writer.writeTextElement(ElementId, exportTemplate.ignoredFields[i]);
                }
            }
            writer.writeEndElement();
        }
        writer.writeEndElement();
    }
    // End document.
    writer.writeEndDocument();
}

void ExportTemplateSerializer::deserialize(QIODevice& device, RecordExportTemplate& exportTemplate) const
{
    // Open device stream.
    XmlReader reader(&device);

    // Validate export template file.
    reader.validate(":/Source/Tome/Features/Export/Model/TomeExportTemplate2.xsd",
                    QObject::tr("Invalid export template file: %1 (line %2, column %3)"));

    // Begin document.
    reader.readStartDocument();
    {
        // Read export flags.
        bool exportAsTable = reader.readAttribute(AttributeExportAsTable) == "true";
        bool exportRoots = reader.readAttribute(AttributeExportRoots) == "true";
        bool exportInnerNodes = reader.readAttribute(AttributeExportInnerNodes) == "true";
        bool exportLeafs = reader.readAttribute(AttributeExportLeafs) == "true";
        bool exportLocalizedFieldsOnly = reader.readAttribute(AttributeExportLocalizedFieldsOnly) == "true";

        exportTemplate.exportAsTable = exportAsTable;
        exportTemplate.exportRoots = exportRoots;
        exportTemplate.exportInnerNodes = exportInnerNodes;
        exportTemplate.exportLeafs = exportLeafs;
        exportTemplate.exportLocalizedFieldsOnly = exportLocalizedFieldsOnly;

        // Read record export templates.
        reader.readStartElement(ElementTemplate);
        {
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

            // Read string replacement map.
            if (reader.isAtElement(ElementStringReplacementMap))
            {
                reader.readStartElement(ElementStringReplacementMap);
                {
                    while (reader.isAtElement(ElementMapping))
                    {
                        reader.readStartElement(ElementMapping);
                        {
                            QString stringReplacementKey = reader.readTextElement(ElementString);
                            QString stringReplacementValue = reader.readTextElement(ElementReplaceWith);

                            exportTemplate.stringReplacementMap.insert(stringReplacementKey, stringReplacementValue);
                        }
                        reader.readEndElement();
                    }
                }
                reader.readEndElement();
            }

            // Read ignore lists.
            reader.readStartElement(ElementIgnoredRecords);
            {
                while (reader.isAtElement(ElementId))
                {
                    exportTemplate.ignoredRecords << reader.readTextElement(ElementId);
                }
            }
            reader.readEndElement();

            reader.readStartElement(ElementIgnoredFields);
            {
                while (reader.isAtElement(ElementId))
                {
                    exportTemplate.ignoredFields << reader.readTextElement(ElementId);
                }
            }
            reader.readEndElement();
        }
        reader.readEndElement();
    }
    // End document.
    reader.readEndDocument();
}
