#include "importtemplateserializer.h"

#include "QXmlStreamWriter"

#include "../Model/recordtableimporttemplate.h"
#include "../../../IO/xmlreader.h"

using namespace Tome;

const QString ImportTemplateSerializer::AttributeColumnName = "ColumnName";
const QString ImportTemplateSerializer::AttributeFieldId = "FieldId";
const QString ImportTemplateSerializer::AttributeKey = "Key";
const QString ImportTemplateSerializer::AttributeValue = "Value";
const QString ImportTemplateSerializer::AttributeVersion = "Version";

const QString ImportTemplateSerializer::ElementColumnMap = "ColumnMap";
const QString ImportTemplateSerializer::ElementId = "Id";
const QString ImportTemplateSerializer::ElementIdColumn = "IdColumn";
const QString ImportTemplateSerializer::ElementIgnoredIds = "IgnoredIds";
const QString ImportTemplateSerializer::ElementMapping = "Mapping";
const QString ImportTemplateSerializer::ElementName = "Name";
const QString ImportTemplateSerializer::ElementParameter = "Parameter";
const QString ImportTemplateSerializer::ElementParameters = "Parameters";
const QString ImportTemplateSerializer::ElementRootRecordId = "RootRecordId";
const QString ImportTemplateSerializer::ElementSourceType = "SourceType";
const QString ImportTemplateSerializer::ElementTemplate = "Template";

const int ImportTemplateSerializer::Version = 1;


ImportTemplateSerializer::ImportTemplateSerializer()
{
}

void ImportTemplateSerializer::serialize(QIODevice& device, const RecordTableImportTemplate& importTemplate) const
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

            // Write name, type, id column and root record.
            writer.writeTextElement(ElementName, importTemplate.name);
            writer.writeTextElement(ElementSourceType, TableType::toString(importTemplate.sourceType));
            writer.writeTextElement(ElementIdColumn, importTemplate.idColumn);
            writer.writeTextElement(ElementRootRecordId, importTemplate.rootRecordId);

            // Write column map.
            writer.writeStartElement(ElementColumnMap);
            {
                for (QMap<QString, QString>::const_iterator itColumnMap = importTemplate.columnMap.cbegin();
                     itColumnMap != importTemplate.columnMap.cend();
                     ++itColumnMap)
                {
                    writer.writeStartElement(ElementMapping);
                    writer.writeAttribute(AttributeColumnName, itColumnMap.key());
                    writer.writeAttribute(AttributeFieldId, itColumnMap.value());
                    writer.writeEndElement();
                }
            }
            writer.writeEndElement();

            // Write ignore list.
            writer.writeStartElement(ElementIgnoredIds);
            {
                for (int i = 0; i < importTemplate.ignoredIds.size(); ++i)
                {
                    writer.writeTextElement(ElementId, importTemplate.ignoredIds[i]);
                }
            }
            writer.writeEndElement();

            // Write parameters.
            writer.writeStartElement(ElementParameters);
            {
                for (QMap<QString, QString>::const_iterator itParameters = importTemplate.parameters.cbegin();
                     itParameters != importTemplate.parameters.cend();
                     ++itParameters)
                {
                    writer.writeStartElement(ElementParameter);
                    writer.writeAttribute(AttributeKey, itParameters.key());
                    writer.writeAttribute(AttributeValue, itParameters.value());
                    writer.writeEndElement();
                }
            }
            writer.writeEndElement();
        }
        writer.writeEndElement();
    }
    writer.writeEndDocument();
}

void ImportTemplateSerializer::deserialize(QIODevice& device, RecordTableImportTemplate& importTemplate) const
{
    // Open device stream.
    QXmlStreamReader streamReader(&device);
    XmlReader reader(streamReader);

    // Begin document.
    reader.readStartDocument();
    {
        // Read record import template.
        reader.readStartElement(ElementTemplate);
        {
            // Read name, type, id column and root record.
            importTemplate.name = reader.readTextElement(ElementName);
            importTemplate.sourceType = TableType::fromString(reader.readTextElement(ElementSourceType));
            importTemplate.idColumn = reader.readTextElement(ElementIdColumn);
            importTemplate.rootRecordId = reader.readTextElement(ElementRootRecordId);

            // Read column map.
            reader.readStartElement(ElementColumnMap);
            {
                while (reader.isAtElement(ElementMapping))
                {
                    QString columnMapKey = reader.readAttribute(AttributeColumnName);
                    QString columnMapValue = reader.readAttribute(AttributeFieldId);

                    importTemplate.columnMap.insert(columnMapKey, columnMapValue);

                    // Advance reader.
                    reader.readEmptyElement(ElementMapping);
                }
            }
            reader.readEndElement();

            // Read ignore list.
            reader.readStartElement(ElementIgnoredIds);
            {
                while (reader.isAtElement(ElementId))
                {
                    importTemplate.ignoredIds << reader.readTextElement(ElementId);
                }
            }
            reader.readEndElement();

            // Read parameters
            reader.readStartElement(ElementParameters);
            {
                while (reader.isAtElement(ElementParameter))
                {
                    QString parameterKey = reader.readAttribute(AttributeKey);
                    QString parameterValue = reader.readAttribute(AttributeValue);

                    importTemplate.parameters.insert(parameterKey, parameterValue);

                    // Advance reader.
                    reader.readEmptyElement(ElementParameter);
                }
            }
            reader.readEndElement();
        }
        reader.readEndElement();
    }
    reader.readEndDocument();
}
