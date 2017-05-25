#ifndef EXPORTCONTROLLER_H
#define EXPORTCONTROLLER_H

#include <QIODevice>
#include <QString>

#include "../Model/recordexporttemplatelist.h"
#include "../Model/recordexporttemplatemap.h"

namespace Tome
{
    class FacetsController;
    class FieldDefinitionsController;
    class RecordsController;
    class TypesController;

    /**
     * @brief Controller for exporting records using templates.
     */
    class ExportController : public QObject
    {
            Q_OBJECT

        public:
        /**
             * @brief Constructs a new controller for exporting records using templates.
             * @param facetsController Controller for validating type facets.
             * @param fieldDefinitionsController Controller for adding, updating and removing field definitions.
             * @param recordsController Controller for adding, updating and removing records.
             * @param typesController Controller for adding, updating and removing custom types.
             */
            ExportController(const FacetsController& facetsController,
                             const FieldDefinitionsController& fieldDefinitionsController,
                             const RecordsController& recordsController,
                             const TypesController& typesController);

            /**
             * @brief Adds a new template for exporting records to the project.
             * @param exportTemplate Record export template to add.
             */
            void addRecordExportTemplate(const RecordExportTemplate& exportTemplate);

            /**
             * @brief Gets the record export template with the specified name.
             *
             * @exception std::out_of_range if the export template could not be found.
             *
             * @see hasRecordExportTemplate for checking whether a record export template exists.
             *
             * @param name Name of the record export template to get.
             * @return Record export template with the specified name.
             */
            const RecordExportTemplate getRecordExportTemplate(const QString& name) const;

            /**
             * @brief Gets a list of all available record export templates of the project.
             * @return List of all available record export templates of the project.
             */
            const RecordExportTemplateList getRecordExportTemplates() const;

            /**
             * @brief Checks whether the project has a record export template with the specified name.
             * @param name Name of the record export template to look for.
             * @return true, if a record export template with the specified name exists, and false otherwise.
             */
            bool hasRecordExportTemplate(const QString& name) const;

            /**
             * @brief Exports all records using the passed export template to a file at the specified path.
             *
             * @exception std::runtime_error if the file at the specified path could not be written.
             *
             * @param exportTemplate Template to apply when exporting the records.
             * @param filePath Path of the file to write the exported data to.
             */
            void exportRecords(const RecordExportTemplate& exportTemplate, const QString& filePath) const;

            /**
             * @brief Exports all records using the passed export template to the specified device.
             * @param exportTemplate Template to apply when exporting the records.
             * @param device Device to write the exported data to.
             */
            void exportRecords(const RecordExportTemplate& exportTemplate, QIODevice& device) const;

            /**
             * @brief Removes the record export template with the specified name from the project.
             * @param name Name of the record export template to remove.
             * @return true, if the export template has been removed, and false otherwise.
             */
            bool removeExportTemplate(const QString& name);

            /**
             * @brief Uses the specified list of record export templates as model for this controller.
             * @param exportTemplates New list of record export templates to use as model for this controller.
             */
            void setRecordExportTemplates(RecordExportTemplateList& exportTemplates);

        signals:
            /**
             * @brief Available record export templates have changed.
             */
            void exportTemplatesChanged();

            /**
             * @brief Progress of the current record export operation has changed.
             * @param title Title of the current export operation step.
             * @param text Details of the current export operation step.
             * @param currentValue Current progress value of the current record export operation step.
             * @param maximumValue Maximum progress value of the current record export operation step.
             */
            void progressChanged(const QString title, const QString text, const int currentValue, const int maximumValue) const;

        private:
            RecordExportTemplateList* model;

            static const QString PlaceholderAppVersion;
            static const QString PlaceholderAppVersionName;
            static const QString PlaceholderComponents;
            static const QString PlaceholderComponentName;
            static const QString PlaceholderExportTime;
            static const QString PlaceholderFieldComponent;
            static const QString PlaceholderFieldDescription;
            static const QString PlaceholderFieldDisplayName;
            static const QString PlaceholderFieldId;
            static const QString PlaceholderFieldKey;
            static const QString PlaceholderFieldType;
            static const QString PlaceholderFieldValue;
            static const QString PlaceholderHash;
            static const QString PlaceholderItemType;
            static const QString PlaceholderKeyType;
            static const QString PlaceholderListItem;
            static const QString PlaceholderRecordDisplayName;
            static const QString PlaceholderRecordFields;
            static const QString PlaceholderRecordId;
            static const QString PlaceholderRecordParentId;
            static const QString PlaceholderRecordRootId;
            static const QString PlaceholderRecords;
            static const QString PlaceholderValueType;

            const FacetsController& facetsController;
            const FieldDefinitionsController& fieldDefinitionsController;
            const RecordsController& recordsController;
            const TypesController& typesController;
    };
}

#endif // EXPORTCONTROLLER_H
