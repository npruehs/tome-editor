#include "exportcontroller.h"

#include <stdexcept>

#include <QFile>
#include <QStringBuilder>
#include <QTextStream>

#include "../../Fields/Model/fielddefinition.h"

using namespace Tome;

const QString ExportController::PlaceholderComponents = "$RECORD_COMPONENTS$";
const QString ExportController::PlaceholderComponentName = "$COMPONENT_NAME$";
const QString ExportController::PlaceholderFieldId = "$FIELD_ID$";
const QString ExportController::PlaceholderFieldType = "$FIELD_TYPE$";
const QString ExportController::PlaceholderFieldValue = "$FIELD_VALUE$";
const QString ExportController::PlaceholderRecordFields = "$RECORD_FIELDS$";
const QString ExportController::PlaceholderRecordId = "$RECORD_ID$";
const QString ExportController::PlaceholderRecords = "$RECORDS$";


ExportController::ExportController(const FieldDefinitionsController& fieldDefinitionsController, const RecordsController& recordsController)
    : fieldDefinitionsController(fieldDefinitionsController),
      recordsController(recordsController)
{
}

const RecordExportTemplate ExportController::getRecordExportTemplate(const QString& name) const
{
    return this->model->value(name);
}

const RecordExportTemplateMap&ExportController::getRecordExportTemplates() const
{
    return *this->model;
}

void ExportController::exportRecords(const RecordExportTemplate& exportTemplate, const QString& filePath)
{
    QFile file(filePath);

    if (file.open(QIODevice::ReadWrite | QIODevice::Truncate))
    {
        this->exportRecords(exportTemplate, file);
    }
    else
    {
        QString errorMessage = QObject::tr("Destination file could not be written:\r\n") + filePath;
        throw std::runtime_error(errorMessage.toStdString());
    }
}

void ExportController::exportRecords(const RecordExportTemplate& exportTemplate, QIODevice& device)
{
    // Build record file string.
    QString recordsString;

    const RecordSetList& recordSets = this->recordsController.getRecordSets();

    for (int i = 0; i < recordSets.size(); ++i)
    {
        const RecordSet& recordSet = recordSets[i];

        for (int j = 0; j < recordSet.records.size(); ++j)
        {
            const Record& record = recordSet.records[j];

            // Build field values string.
            QString fieldValuesString;

            for (QMap<QString, QString>::const_iterator itFields = record.fieldValues.begin();
                 itFields != record.fieldValues.end();
                 ++itFields)
            {
                QString fieldId = itFields.key();
                QString fieldValue = itFields.value();
                const FieldDefinition& fieldDefinition = this->fieldDefinitionsController.getFieldDefinition(fieldId);

                // Get field type name.
                QString fieldType = fieldDefinition.fieldType;

                if (exportTemplate.typeMap.contains(fieldType))
                {
                    fieldType = exportTemplate.typeMap[fieldType];
                }

                // Apply field value template.
                QString fieldValueString = exportTemplate.fieldValueTemplate;
                fieldValueString = fieldValueString.replace(PlaceholderFieldId, fieldId);
                fieldValueString = fieldValueString.replace(PlaceholderFieldType, fieldType);
                fieldValueString = fieldValueString.replace(PlaceholderFieldValue, fieldValue);

                fieldValuesString.append(fieldValueString);

                // Add delimiter, if necessary.
                if (itFields != record.fieldValues.end() - 1)
                {
                    fieldValuesString.append(exportTemplate.fieldValueDelimiter);
                }
            }

            // Collect components.
            QStringList components;

            for (QMap<QString, QString>::const_iterator itFields = record.fieldValues.begin();
                 itFields != record.fieldValues.end();
                 ++itFields)
            {
                QString fieldId = itFields.key();
                const FieldDefinition& fieldDefinition = this->fieldDefinitionsController.getFieldDefinition(fieldId);

                if (!fieldDefinition.component.isEmpty() && !components.contains(fieldDefinition.component))
                {
                    components.append(fieldDefinition.component);
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
                QString componentString = exportTemplate.componentTemplate;
                componentString = componentString.replace(PlaceholderComponentName, component);

                componentsString.append(componentString);

                // Add delimiter, if necessary.
                if (itComponents != components.end() - 1)
                {
                    componentsString.append(exportTemplate.componentDelimiter);
                }
            }

            // Apply record template.
            QString recordString = exportTemplate.recordTemplate;
            recordString = recordString.replace(PlaceholderRecordId, record.id);
            recordString = recordString.replace(PlaceholderRecordFields, fieldValuesString);
            recordString = recordString.replace(PlaceholderComponents, componentsString);

            recordsString.append(recordString);

            if (j < recordSet.records.size() - 1 || i < recordSets.size() - 1)
            {
                recordsString.append(exportTemplate.recordDelimiter);
            }
        }
    }

    // Apply record file template.
    QString recordFileString = exportTemplate.recordFileTemplate;
    recordFileString = recordFileString.replace(PlaceholderRecords, recordsString);

    // Write record file.
    QTextStream textStream(&device);
    textStream << recordFileString;
}

void ExportController::setRecordExportTemplates(RecordExportTemplateMap& model)
{
    this->model = &model;
}
