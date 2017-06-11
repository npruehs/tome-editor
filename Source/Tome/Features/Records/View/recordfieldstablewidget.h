#ifndef RECORDFIELDSTABLEWIDGET_H
#define RECORDFIELDSTABLEWIDGET_H

#include <QTableWidget>

namespace Tome
{
    class FacetsController;
    class FieldDefinition;
    class FieldDefinitionsController;
    class ProjectController;
    class RecordsController;
    class TypesController;

    /**
     * @brief Allows editing the values of all fields of a record.
     */
    class RecordFieldsTableWidget : public QTableWidget
    {
        Q_OBJECT

        public:
            /**
             * @brief Constructs a new widget for editing the values of all fields of a record.
             * @param fieldDefinitionsController Controller for adding, updating and removing field definitions.
             * @param facetsController Controller for validating type facets.
             * @param projectController Controller for creating, loading and saving projects.
             * @param recordsController Controller for adding, updating and removing records.
             * @param typesController Controller for adding, updating and removing custom types.
             */
            RecordFieldsTableWidget(FieldDefinitionsController& fieldDefinitionsController,
                                    FacetsController& facetsController,
                                    ProjectController& projectController,
                                    RecordsController& recordsController,
                                    TypesController& typesController);

            /**
             * @brief Sets whether to show an additional column with the description of all record fields, or not.
             * @param enabled Whether to show an additional column with the description of all record fields, or not.
             */
            void setDescriptionColumnEnabled(bool enabled);

            /**
             * @brief Sets the record to edit the field values of.
             * @param recordId Id of the record to edit the field values of.
             */
            void setRecord(const QVariant recordId);

            /**
             * @brief Sets whether to show the names of the respective component in front of the field name, or not.
             * @param showComponentNames Whether to show the names of the respective component in front of the field name, or not.
             */
            void setShowComponentNames(bool showComponentNames);

            /**
             * @brief Updates the specified row of this record field table.
             * @param i Index of the row to update.
             */
            void updateFieldValue(int i);

        signals:
            /**
             * @brief A link to a file on disk has been activated.
             * @param filePath Path to the selected file on disk.
             */
            void fileLinkActivated(const QString& filePath);

            /**
             * @brief A link to another Tome record has been activated.
             * @param recordId Id of the selected record.
             */
            void recordLinkActivated(const QString& recordId);

        private:
            FacetsController& facetsController;
            FieldDefinitionsController& fieldDefinitionsController;
            ProjectController& projectController;
            RecordsController& recordsController;
            TypesController& typesController;

            QVariant recordId;
            bool showComponentNames;

            const QString getFieldKeyString(const FieldDefinition& field);

        private slots:
            void onFileLinkActivated(const QString& filePath);
            void onRecordLinkActivated(const QString& recordId);
    };
}

#endif // RECORDFIELDSTABLEWIDGET_H
