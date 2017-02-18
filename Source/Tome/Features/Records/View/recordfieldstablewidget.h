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

    class RecordFieldsTableWidget : public QTableWidget
    {
        Q_OBJECT

        public:
            RecordFieldsTableWidget(FieldDefinitionsController& fieldDefinitionsController,
                                    FacetsController& facetsController,
                                    ProjectController& projectController,
                                    RecordsController& recordsController,
                                    TypesController& typesController);

            void setDescriptionColumnEnabled(bool enabled);
            void setRecord(const QString recordId);
            void setShowComponentNames(bool showComponentNames);
            void updateFieldValue(int i);

        signals:
            void fileLinkActivated(const QString& filePath);
            void recordLinkActivated(const QString& recordId);

        private:
            FacetsController& facetsController;
            FieldDefinitionsController& fieldDefinitionsController;
            ProjectController& projectController;
            RecordsController& recordsController;
            TypesController& typesController;

            QString recordId;
            bool showComponentNames;

            const QString getFieldKeyString(const FieldDefinition& field);

        private slots:
            void onFileLinkActivated(const QString& filePath);
            void onRecordLinkActivated(const QString& recordId);
    };
}

#endif // RECORDFIELDSTABLEWIDGET_H
