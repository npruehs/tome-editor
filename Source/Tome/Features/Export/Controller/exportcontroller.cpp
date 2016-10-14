#include "exportcontroller.h"

#include <stdexcept>

#include <QFile>
#include <QStringBuilder>
#include <QTextStream>

#include "../../Fields//Controller/fielddefinitionscontroller.h"
#include "../../Fields/Model/fielddefinition.h"
#include "../../Records/Controller/recordscontroller.h"
#include "../../Types/Controller/typescontroller.h"
#include "../../Types/Model/builtintype.h"
#include "../../Types/Model/vector.h"

using namespace Tome;

const QString ExportController::PlaceholderComponents = "$RECORD_COMPONENTS$";
const QString ExportController::PlaceholderComponentName = "$COMPONENT_NAME$";
const QString ExportController::PlaceholderItemType = "$ITEM_TYPE$";
const QString ExportController::PlaceholderFieldComponent = "$FIELD_COMPONENT$";
const QString ExportController::PlaceholderFieldDescription = "$FIELD_DESCRIPTION$";
const QString ExportController::PlaceholderFieldDisplayName = "$FIELD_DISPLAY_NAME$";
const QString ExportController::PlaceholderFieldId = "$FIELD_ID$";
const QString ExportController::PlaceholderFieldKey = "$FIELD_KEY$";
const QString ExportController::PlaceholderFieldType = "$FIELD_TYPE$";
const QString ExportController::PlaceholderFieldValue = "$FIELD_VALUE$";
const QString ExportController::PlaceholderKeyType = "$KEY_TYPE$";
const QString ExportController::PlaceholderListItem = "$LIST_ITEM$";
const QString ExportController::PlaceholderRecordDisplayName = "$RECORD_DISPLAY_NAME$";
const QString ExportController::PlaceholderRecordFields = "$RECORD_FIELDS$";
const QString ExportController::PlaceholderRecordId = "$RECORD_ID$";
const QString ExportController::PlaceholderRecordParentId = "$RECORD_PARENT$";
const QString ExportController::PlaceholderRecords = "$RECORDS$";
const QString ExportController::PlaceholderValueType = "$VALUE_TYPE$";


ExportController::ExportController(const FieldDefinitionsController& fieldDefinitionsController, const RecordsController& recordsController, const TypesController& typesController)
    : fieldDefinitionsController(fieldDefinitionsController),
      recordsController(recordsController),
      typesController(typesController)
{
}

void ExportController::addRecordExportTemplate(const RecordExportTemplate& exportTemplate)
{
    // Update model.
    this->model->push_back(exportTemplate);

    // Notify listeners.
    emit this->exportTemplatesChanged();
}

const RecordExportTemplate ExportController::getRecordExportTemplate(const QString& name) const
{
    for (RecordExportTemplateList::iterator it = this->model->begin();
         it != this->model->end();
         ++it)
    {
        if (it->name == name)
        {
            return *it;
        }
    }

    QString errorMessage = QObject::tr("Export template not  found: ") + name;
    throw std::runtime_error(errorMessage.toStdString());
}

const RecordExportTemplateList ExportController::getRecordExportTemplates() const
{
    return *this->model;
}

bool ExportController::hasRecordExportTemplate(const QString& name) const
{
    for (RecordExportTemplateList::iterator it = this->model->begin();
         it != this->model->end();
         ++it)
    {
        if (it->name == name)
        {
            return true;
        }
    }

    return false;
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
    const FieldDefinitionList& fields = this->fieldDefinitionsController.getFieldDefinitions();

    for (int i = 0; i < recordSets.size(); ++i)
    {
        const RecordSet& recordSet = recordSets[i];

        for (int j = 0; j < recordSet.records.size(); ++j)
        {
            const Record& record = recordSet.records[j];

            // Check if should export.
            if (record.parentId.isEmpty())
            {
                // Root node.
                if (!exportTemplate.exportRoots)
                {
                    continue;
                }
            }
            else
            {
                if (this->recordsController.getChildren(record.id).empty())
                {
                    // Leaf node.
                    if (!exportTemplate.exportLeafs)
                    {
                        continue;
                    }
                }
                else
                {
                    // Inner node.
                    if (!exportTemplate.exportInnerNodes)
                    {
                        continue;
                    }
                }
            }

            // Check if ignored.
            if (exportTemplate.ignoredRecords.contains(record.id))
            {
                continue;
            }

            // Check if any ancestor ignored.
            RecordList ancestors = this->recordsController.getAncestors(record.id);
            bool anyAncestorIgnored = false;

            for (int i = 0; i < ancestors.size(); ++i)
            {
                if (exportTemplate.ignoredRecords.contains(ancestors[i].id))
                {
                    anyAncestorIgnored = true;
                    break;
                }
            }

            if (anyAncestorIgnored)
            {
                continue;
            }

            // Build field values string.
            QString fieldValuesString;

            // Get fields to export.
            RecordFieldValueMap fieldValues = recordsController.getRecordFieldValues(record.id);

            if (exportTemplate.exportAsTable)
            {
                // Build field table, filling up with empty values.
                for (int k = 0; k < fields.count(); ++k)
                {
                    const FieldDefinition& field = fields[k];

                    if (!fieldValues.contains(field.id))
                    {
                        fieldValues[field.id] = "";
                    }
                }
            }

            // Do not export empty records.
            if (fieldValues.empty())
            {
                continue;
            }

            // Build field value text representations.
            QMap<QString, QString> fieldValueTexts;

            for (RecordFieldValueMap::iterator itFields = fieldValues.begin();
                 itFields != fieldValues.end();
                 ++itFields)
            {
                QString fieldId = itFields.key();
                QVariant fieldValue = itFields.value();
                QString fieldValueText = fieldValue.toString();

                const FieldDefinition& fieldDefinition = this->fieldDefinitionsController.getFieldDefinition(fieldId);

                // Get field type name.
                QString fieldType = fieldDefinition.fieldType;

                // Check if list.
                if (this->typesController.isCustomType(fieldType))
                {
                    const CustomType& customType = this->typesController.getCustomType(fieldType);

                    if (customType.isList())
                    {
                        QString itemType = customType.getItemType();
                        QString exportedItemType = exportTemplate.typeMap.value(itemType, itemType);
                        // Build list string.
                        fieldValueText = QString();

                        QVariantList list = fieldValue.toList();

                        for (int i = 0; i < list.size(); ++i)
                        {
                            QString listItem = exportTemplate.listItemTemplate;
                            listItem = listItem.replace(PlaceholderFieldId, fieldId);
                            listItem = listItem.replace(PlaceholderItemType, exportedItemType);
                            listItem = listItem.replace(PlaceholderListItem, list[i].toString());
                            fieldValueText.append(listItem);

                            if (i < list.size() - 1)
                            {
                                fieldValueText.append(exportTemplate.listItemDelimiter);
                            }
                        }
                    }
                    else if (customType.isMap())
                    {
                        // Build map string.
                        fieldValueText = QString();

                        QVariantMap map = fieldValue.toMap();

                        for (QVariantMap::const_iterator it = map.begin();
                             it != map.end();
                             ++it)
                        {
                            QString mapItem = exportTemplate.mapItemTemplate;
                            mapItem = mapItem.replace(PlaceholderFieldId, fieldId);
                            mapItem = mapItem.replace(PlaceholderFieldKey, it.key());
                            mapItem = mapItem.replace(PlaceholderFieldValue, QVariant(it.value()).toString());
                            fieldValueText.append(mapItem);

                            if (it + 1 != map.end())
                            {
                                fieldValueText.append(exportTemplate.mapItemDelimiter);
                            }
                        }
                    }
                }
                // Check if vector.
                else if (fieldType == BuiltInType::Vector2I || fieldType == BuiltInType::Vector2R ||
                         fieldType == BuiltInType::Vector3I || fieldType == BuiltInType::Vector3R)
                {
                    // Build vector string.
                    fieldValueText = QString();

                    QVariantMap vector = fieldValue.toMap();

                    QVariant x = vector[BuiltInType::Vector::X];
                    QVariant y = vector[BuiltInType::Vector::Y];

                    // X.
                    QString vectorComponent = exportTemplate.mapItemTemplate;
                    vectorComponent = vectorComponent.replace(PlaceholderFieldId, fieldId);
                    vectorComponent = vectorComponent.replace(PlaceholderFieldKey, "X");
                    vectorComponent = vectorComponent.replace(PlaceholderFieldValue, x.toString());
                    fieldValueText.append(vectorComponent);
                    fieldValueText.append(exportTemplate.mapItemDelimiter);

                    // Y.
                    vectorComponent = exportTemplate.mapItemTemplate;
                    vectorComponent = vectorComponent.replace(PlaceholderFieldId, fieldId);
                    vectorComponent = vectorComponent.replace(PlaceholderFieldKey, "Y");
                    vectorComponent = vectorComponent.replace(PlaceholderFieldValue, y.toString());
                    fieldValueText.append(vectorComponent);

                    if (fieldType == BuiltInType::Vector3I || fieldType == BuiltInType::Vector3R)
                    {
                        QVariant z = vector[BuiltInType::Vector::Z];

                        // Z.
                        fieldValueText.append(exportTemplate.mapItemDelimiter);

                        vectorComponent = exportTemplate.mapItemTemplate;
                        vectorComponent = vectorComponent.replace(PlaceholderFieldId, fieldId);
                        vectorComponent = vectorComponent.replace(PlaceholderFieldKey, "Z");
                        vectorComponent = vectorComponent.replace(PlaceholderFieldValue, z.toString());
                        fieldValueText.append(vectorComponent);
                    }
                }

                // Store for later use.
                fieldValueTexts[fieldId] = fieldValueText;
            }

            // Apply record template.
            QString recordString = exportTemplate.recordTemplate;

            // Use regular expressions to match specific fields.
            QRegExp regEx("\\$FIELD_VALUE:([a-zA-Z]*)\\$");
            QStringList matchedSpecificFields;

            int pos = 0;
            while ((pos = regEx.indexIn(recordString, pos)) != -1)
            {
                QString fieldId = regEx.cap(1);
                QString fieldValue = fieldValueTexts[fieldId];

                recordString.replace(pos, regEx.matchedLength(), fieldValue);
                pos += fieldValue.length();

                // Remember this field has already been matched, so it can be omitted later.
                matchedSpecificFields << fieldId;
            }

            // Build full field values string.
            for (RecordFieldValueMap::iterator itFields = fieldValues.begin();
                 itFields != fieldValues.end();
                 ++itFields)
            {
                const QString fieldId = itFields.key();

                if (matchedSpecificFields.contains(fieldId))
                {
                    // Field has already explicitly been matched by a regular expression before. Skip.
                    continue;
                }

                if (exportTemplate.ignoredFields.contains(fieldId))
                {
                    // Field ignored by template.
                    continue;
                }

                const QString fieldValueText = fieldValueTexts[fieldId];
                const FieldDefinition& fieldDefinition = this->fieldDefinitionsController.getFieldDefinition(fieldId);

                // Get field type name.
                const QString fieldType = fieldDefinition.fieldType;
                const QString exportedFieldType = exportTemplate.typeMap.value(fieldType, fieldType);
                const QString fieldComponent = fieldDefinition.component;
                const QString fieldDescription = fieldDefinition.description;
                const QString fieldDisplayName = fieldDefinition.displayName;

                // Apply field value template.
                QString fieldValueString = exportTemplate.fieldValueTemplate;

                // Check if list.
                if (this->typesController.isCustomType(fieldType))
                {
                    const CustomType& customType = this->typesController.getCustomType(fieldType);

                    if (customType.isList())
                    {
                        // Use list template.
                        fieldValueString = exportTemplate.listTemplate;

                        QString itemType = customType.getItemType();
                        QString exportedItemType = exportTemplate.typeMap.value(itemType, itemType);

                        fieldValueString = fieldValueString.replace(PlaceholderItemType, exportedItemType);
                    }
                    else if (customType.isMap())
                    {
                        // Use map template.
                        fieldValueString = exportTemplate.mapTemplate;

                        QString keyType = customType.getKeyType();
                        QString valueType = customType.getValueType();

                        QString exportedKeyType = exportTemplate.typeMap.value(keyType, keyType);
                        QString exportedValueType = exportTemplate.typeMap.value(valueType, valueType);

                        fieldValueString = fieldValueString.replace(PlaceholderKeyType, exportedKeyType);
                        fieldValueString = fieldValueString.replace(PlaceholderValueType, exportedValueType);
                    }
                }
                // Check if vector.
                else if (fieldType == BuiltInType::Vector2I || fieldType == BuiltInType::Vector2R ||
                         fieldType == BuiltInType::Vector3I || fieldType == BuiltInType::Vector3R)
                {
                    // Use vector template.
                    fieldValueString = exportTemplate.mapTemplate;

                    QString exportedKeyType = exportTemplate.typeMap.value("String", "String");
                    QString exportedValueType;

                    if (fieldType == BuiltInType::Vector2I || fieldType == BuiltInType::Vector3I)
                    {
                        exportedValueType = exportTemplate.typeMap.value("Integer", "Integer");
                    }
                    else
                    {
                        exportedValueType = exportTemplate.typeMap.value("Real", "Real");
                    }

                    fieldValueString = fieldValueString.replace(PlaceholderKeyType, exportedKeyType);
                    fieldValueString = fieldValueString.replace(PlaceholderValueType, exportedValueType);
                }

                fieldValueString = fieldValueString.replace(PlaceholderFieldId, fieldId);
                fieldValueString = fieldValueString.replace(PlaceholderFieldType, exportedFieldType);
                fieldValueString = fieldValueString.replace(PlaceholderFieldValue, fieldValueText);
                fieldValueString = fieldValueString.replace(PlaceholderFieldComponent, fieldComponent);
                fieldValueString = fieldValueString.replace(PlaceholderFieldDisplayName, fieldDisplayName);
                fieldValueString = fieldValueString.replace(PlaceholderFieldDescription, fieldDescription);

                fieldValuesString.append(fieldValueString);

                // Add delimiter, if necessary.
                if (itFields != fieldValues.end() - 1)
                {
                    fieldValuesString.append(exportTemplate.fieldValueDelimiter);
                }
            }

            // Collect components.
            QStringList components;

            for (QMap<QString, QVariant>::const_iterator itFields = fieldValues.begin();
                 itFields != fieldValues.end();
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

            // Only export record parent if that parent isn't empty.
            QString recordParent;

            if (!record.parentId.isEmpty())
            {
                RecordFieldValueMap parentFieldValues = recordsController.getRecordFieldValues(record.parentId);

                if (!parentFieldValues.empty())
                {
                    recordParent = record.parentId;
                }
            }

            // Replace other record placeholders.
            recordString = recordString.replace(PlaceholderRecordId, record.id);
            recordString = recordString.replace(PlaceholderRecordParentId, recordParent);
            recordString = recordString.replace(PlaceholderRecordFields, fieldValuesString);
            recordString = recordString.replace(PlaceholderComponents, componentsString);
            recordString = recordString.replace(PlaceholderRecordDisplayName, record.displayName);

            if (!recordsString.isEmpty())
            {
                // Any previous record export succeeded (e.g. wasn't skipped). Add delimiter.
                recordsString.append(exportTemplate.recordDelimiter);
            }

            recordsString.append(recordString);
        }
    }

    // Apply record file template.
    QString recordFileString = exportTemplate.recordFileTemplate;
    recordFileString = recordFileString.replace(PlaceholderRecords, recordsString);

    // Write record file.
    QTextStream textStream(&device);
    textStream.setCodec("UTF-8");
    textStream << recordFileString;
}

void ExportController::removeExportTemplate(const QString& name)
{
    // Update model.
    for (RecordExportTemplateList::iterator it = this->model->begin();
         it != this->model->end();
         ++it)
    {
        if (it->name == name)
        {
            this->model->erase(it);

            // Notify listeners.
            emit this->exportTemplatesChanged();
            return;
        }
    }
}

void ExportController::setRecordExportTemplates(RecordExportTemplateList& exportTemplates)
{
    this->model = &exportTemplates;
}
