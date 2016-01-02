#include "recordexporter.h"

using namespace Tome;

#include <QStringBuilder>
#include <QTextStream>

#include "../Values/valueconverter.h"


const QString RecordExporter::PlaceholderComponents = "$RECORD_COMPONENTS$";
const QString RecordExporter::PlaceholderComponentName = "$COMPONENT_NAME$";
const QString RecordExporter::PlaceholderFieldId = "$FIELD_ID$";
const QString RecordExporter::PlaceholderFieldType = "$FIELD_TYPE$";
const QString RecordExporter::PlaceholderFieldValue = "$FIELD_VALUE$";
const QString RecordExporter::PlaceholderRecordFields = "$RECORD_FIELDS$";
const QString RecordExporter::PlaceholderRecordId = "$RECORD_ID$";
const QString RecordExporter::PlaceholderRecords = "$RECORDS$";


RecordExporter::RecordExporter()
{
}

void RecordExporter::exportRecords(QSharedPointer<QIODevice> device, QSharedPointer<Project> project, QSharedPointer<RecordExportTemplate> exportTemplate)
{
    ValueConverter valueConverter;

    // Build record file string.
    QString recordsString;

    for (QVector<QSharedPointer<RecordSet> >::iterator itRecordSets = project->recordSets.begin();
         itRecordSets != project->recordSets.end();
         ++itRecordSets)
    {
        QSharedPointer<RecordSet> recordSet = *itRecordSets;

        for (QVector<QSharedPointer<Record> >::iterator itRecords = recordSet->records.begin();
             itRecords != recordSet->records.end();
             ++itRecords)
        {
            QSharedPointer<Record> record = *itRecords;

            // Build field values string.
            QString fieldValuesString;

            for (QMap<QString, QString>::iterator itFields = record->fieldValues.begin();
                 itFields != record->fieldValues.end();
                 ++itFields)
            {
                QString fieldId = itFields.key();
                QString fieldValue = itFields.value();
                QSharedPointer<FieldDefinition> fieldDefinition = project->getFieldDefinition(fieldId);

                // Apply field value template.
                QString fieldValueString = exportTemplate->fieldValueTemplate;
                fieldValueString = fieldValueString.replace(PlaceholderFieldId, fieldId);
                fieldValueString = fieldValueString.replace(PlaceholderFieldType, valueConverter.FieldTypeToString(fieldDefinition->fieldType));
                fieldValueString = fieldValueString.replace(PlaceholderFieldValue, fieldValue);

                fieldValuesString.append(fieldValueString);

                // Add delimiter, if necessary.
                if (itFields != record->fieldValues.end() - 1)
                {
                    fieldValuesString.append(exportTemplate->fieldValueDelimiter);
                }
            }

            // Collect components.
            QStringList components;

            for (QMap<QString, QString>::iterator itFields = record->fieldValues.begin();
                 itFields != record->fieldValues.end();
                 ++itFields)
            {
                QString fieldId = itFields.key();
                QSharedPointer<FieldDefinition> fieldDefinition = project->getFieldDefinition(fieldId);

                if (!fieldDefinition->component.isEmpty() && !components.contains(fieldDefinition->component))
                {
                    components.append(fieldDefinition->component);
                }
            }

            // Build components string.
            QString componentsString;

            for (QStringList::iterator itComponents = components.begin();
                 itComponents != components.end();
                 ++itComponents)
            {
                QString& component = *itComponents;

                // Apply component template.
                QString componentString = exportTemplate->componentTemplate;
                componentString = componentString.replace(PlaceholderComponentName, component);

                componentsString.append(componentString);

                // Add delimiter, if necessary.
                if (itComponents != components.end() - 1)
                {
                    componentsString.append(exportTemplate->componentDelimiter);
                }
            }

            // Apply record template.
            QString recordString = exportTemplate->recordTemplate;
            recordString = recordString.replace(PlaceholderRecordId, record->id);
            recordString = recordString.replace(PlaceholderRecordFields, fieldValuesString);
            recordString = recordString.replace(PlaceholderComponents, componentsString);

            recordsString.append(recordString);

            if (itRecords != recordSet->records.end() - 1 || itRecordSets != project->recordSets.end() - 1)
            {
                recordsString.append(exportTemplate->recordDelimiter);
            }
        }
    }

    // Apply record file template.
    QString recordFileString = exportTemplate->recordFileTemplate;
    recordFileString = recordFileString.replace(PlaceholderRecords, recordsString);

    // Write record file.
    QTextStream textStream(device.data());
    textStream << recordFileString;
}
