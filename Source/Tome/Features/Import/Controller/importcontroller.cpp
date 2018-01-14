#include "importcontroller.h"

using namespace Tome;

#include <QFileInfo>

#include "recorddatasource.h"
#include "csvrecorddatasource.h"
#include "googlesheetsrecorddatasource.h"
#include "xlsxrecorddatasource.h"
#include "../../Fields/Controller/fielddefinitionscontroller.h"
#include "../../Records/Controller/recordscontroller.h"
#include "../../Types/Controller/typescontroller.h"


ImportController::ImportController(FieldDefinitionsController& fieldDefinitionsController, RecordsController& recordsController, TypesController& typesController)
    : fieldDefinitionsController(fieldDefinitionsController),
      recordsController(recordsController),
      typesController(typesController)
{
}

void ImportController::addRecordImportTemplate(const RecordTableImportTemplate& importTemplate)
{
    qInfo(qUtf8Printable(QString("Adding import template %1.").arg(importTemplate.name)));

    // Update model.
    this->model->push_back(importTemplate);

    // Notify listeners.
    emit this->importTemplatesChanged();
}

const RecordTableImportTemplate ImportController::getRecordTableImportTemplate(const QString& name) const
{
    for (RecordTableImportTemplateList::iterator it = this->model->begin();
         it != this->model->end();
         ++it)
    {
        if (it->name == name)
        {
            return *it;
        }
    }

    QString errorMessage = QObject::tr("Import template not  found: ") + name;
    qCritical(qUtf8Printable(errorMessage));
    throw std::out_of_range(errorMessage.toStdString());
}

const RecordTableImportTemplateList ImportController::getRecordTableImportTemplates() const
{
    return *this->model;
}

void ImportController::importRecords(const RecordTableImportTemplate& importTemplate, const QVariant& context)
{
    qInfo(qUtf8Printable(QString("Importing data from %1 with import template %2.")
             .arg(context.toString(), importTemplate.name)));

    // Create data source.
    RecordDataSource* dataSource;

    switch (importTemplate.sourceType)
    {
        case TableType::Csv:
            dataSource = new CsvRecordDataSource();
            break;

        case TableType::GoogleSheets:
            dataSource = new GoogleSheetsRecordDataSource();
            break;

        case TableType::Xlsx:
            dataSource = new XlsxRecordDataSource();
            break;

        default:
            emit this->importError(tr("Unknown import type."));
            return;
    }

    // Read data from source.
    connect(dataSource,
            SIGNAL(dataAvailable(const QString&, const QVariant&, const QMap<QString,RecordFieldValueMap>&)),
            SLOT(onDataAvailable(const QString&, const QVariant&, const QMap<QString,RecordFieldValueMap>&)));

    connect(dataSource,
            SIGNAL(dataUnavailable(const QString&, const QVariant&, const QString&)),
            SLOT(onDataUnavailable(const QString&, const QVariant&, const QString&)));

    connect(dataSource,
            SIGNAL(progressChanged(const QString, const QString, const int, const int)),
            SLOT(onProgressChanged(const QString, const QString, const int, const int)));

    // Notify listeners.
    emit this->importStarted();

    // Start import.
    dataSource->importData(importTemplate, context);
}

bool ImportController::removeImportTemplate(const QString& name)
{
    qInfo(qUtf8Printable(QString("Removing import template %1.").arg(name)));

    // Update model.
    for (RecordTableImportTemplateList::iterator it = this->model->begin();
         it != this->model->end();
         ++it)
    {
        if (it->name == name)
        {
            this->model->erase(it);

            // Notify listeners.
            emit this->importTemplatesChanged();
            return true;
        }
    }

    return false;
}

void ImportController::setRecordTableImportTemplates(RecordTableImportTemplateList& importTemplates)
{
    this->model = &importTemplates;
}

void ImportController::onDataAvailable(const QString& importTemplateName, const QVariant& context, const QMap<QString, RecordFieldValueMap>& data) const
{
    // Setup parameters.
    const RecordTableImportTemplate& importTemplate = this->getRecordTableImportTemplate(importTemplateName);
    const QStringList& recordSetNames = this->recordsController.getRecordSetNames();
    const QString& recordSetName = recordSetNames.first();

    QString contextName;

    switch (importTemplate.sourceType)
    {
        case TableType::Csv:
        case TableType::Xlsx:
        {
            QFileInfo sourceFile = QFileInfo(context.toString());
            contextName = sourceFile.fileName();
            break;
        }

        case TableType::GoogleSheets:
            contextName = context.toString();
            break;

        default:
            contextName = tr("Unknown");
            break;
    }

    // Update records.
    int recordsAdded = 0;
    int fieldsUpdated = 0;
    int fieldsSkipped = 0;
    int fieldsUpToDate = 0;

    // Show progress bar.
    QString progressBarTitle = tr("Importing %1 With %2").arg(contextName, importTemplateName);
    int index = 0;

    for (QMap<QString, RecordFieldValueMap>::const_iterator itRecords = data.cbegin();
         itRecords != data.cend();
         ++itRecords)
    {
        ++index;

        // Get record.
        const QString& recordId = itRecords.key();
        const RecordFieldValueMap& newRecordFieldValues = itRecords.value();

        if (recordId.isEmpty())
        {
            continue;
        }

        // Update progress bar.
        emit this->progressChanged(progressBarTitle, recordId, index, data.count());

        // Get record display name and editor icon, if available.
        QVariant recordDisplayName;
        QVariant recordEditorIconFieldId;

        if (newRecordFieldValues.contains(importTemplate.displayNameColumn))
        {
            recordDisplayName = newRecordFieldValues[importTemplate.displayNameColumn];
        }
        else
        {
            recordDisplayName = recordId;
        }

        if (newRecordFieldValues.contains(importTemplate.editorIconFieldIdColumn))
        {
            recordEditorIconFieldId = newRecordFieldValues[importTemplate.editorIconFieldIdColumn];
        }

        // Check if need to add new record.
        if (!this->recordsController.hasRecord(recordId))
        {
            // make sure the parent record exists.
            if (!this->recordsController.hasRecord(importTemplate.rootRecordId))
            {
                this->recordsController.addRecord(importTemplate.rootRecordId, importTemplate.rootRecordId, QString(), QStringList(), recordSetName);
                ++recordsAdded;
            }
            this->recordsController.addRecord(recordId, recordDisplayName.toString(), recordEditorIconFieldId.toString(), QStringList(), recordSetName);
            this->recordsController.reparentRecord(recordId, importTemplate.rootRecordId);
            ++recordsAdded;
        }
        else
        {
            qInfo(qUtf8Printable(QString("Updating record %1.").arg(recordId)));

            if (recordDisplayName.isValid())
            {
                this->recordsController.setRecordDisplayName(recordId, recordDisplayName.toString());
            }

            if (recordEditorIconFieldId.isValid())
            {
                this->recordsController.setRecordEditorIconFieldId(recordId, recordEditorIconFieldId.toString());
            }
        }

        // Get current record field values.
        const RecordFieldValueMap oldRecordFieldValues = this->recordsController.getRecordFieldValues(recordId);

        for (RecordFieldValueMap::const_iterator itFields = newRecordFieldValues.cbegin();
             itFields != newRecordFieldValues.cend();
             ++itFields)
        {
            // Get field.
            QString fieldId = itFields.key();
            QVariant fieldValue = itFields.value();

            // Check if field is mapped.
            if (importTemplate.columnMap.contains(fieldId))
            {
                fieldId = importTemplate.columnMap[fieldId];
            }

            if (!this->fieldDefinitionsController.hasFieldDefinition(fieldId))
            {
                qWarning(qUtf8Printable(QString("Skipping unknown field: %1").arg(fieldId)));
                ++fieldsSkipped;
                continue;
            }

            // Apply string replacement.
            for (auto itStringReplacementMap = importTemplate.stringReplacementMap.cbegin();
                 itStringReplacementMap != importTemplate.stringReplacementMap.cend();
                 ++itStringReplacementMap)
            {
                QString fieldValueString = fieldValue.toString();

                if (fieldValueString.contains(itStringReplacementMap.key()))
                {
                    fieldValue = fieldValueString.replace(itStringReplacementMap.key(), itStringReplacementMap.value());
                }
            }

            // Convert to list if necessary.
            const FieldDefinition& field = this->fieldDefinitionsController.getFieldDefinition(fieldId);
            bool isList = this->typesController.isCustomType(field.fieldType) && this->typesController.getCustomType(field.fieldType).isList();

            if (isList)
            {
                fieldValue = fieldValue.toString().split(",");
            }

            // Check if needs update.
            if (oldRecordFieldValues.contains(fieldId) && oldRecordFieldValues[fieldId] == fieldValue)
            {
                ++fieldsUpToDate;
            }
            else
            {
                this->recordsController.updateRecordFieldValue(recordId, fieldId, fieldValue);
                ++fieldsUpdated;
            }
        }
    }

    qInfo(qUtf8Printable(QString("Import finished. %1 new records added. %2 field values updated, %3 skipped, %4 up-to-date.")
          .arg(QString::number(recordsAdded),
               QString::number(fieldsUpdated),
               QString::number(fieldsSkipped),
               QString::number(fieldsUpToDate))));

    emit this->importFinished();
}

void ImportController::onDataUnavailable(const QString& importTemplateName, const QVariant& context, const QString& error) const
{
    Q_UNUSED(importTemplateName)
    Q_UNUSED(context)

    // Show error message.
    emit this->importError(error);

    // Hide progress bar.
    this->onProgressChanged(QString(), QString(), 1, 1);
    emit this->importFinished();
}

void ImportController::onProgressChanged(const QString title, const QString text, const int currentValue, const int maximumValue) const
{
    emit this->progressChanged(title, text, currentValue, maximumValue);
}
