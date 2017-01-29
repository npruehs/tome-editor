#ifndef RECORDFIELDSTABLEWIDGET_H
#define RECORDFIELDSTABLEWIDGET_H

#include <QTableWidget>

namespace Tome
{
    class FieldDefinition;
    class FieldDefinitionsController;
    class RecordsController;
    class TypesController;

    class RecordFieldsTableWidget : public QTableWidget
    {
        Q_OBJECT

        public:
            RecordFieldsTableWidget(FieldDefinitionsController& fieldDefinitionsController, RecordsController& recordsController, TypesController& typesController);

            void setDescriptionColumnEnabled(bool enabled);
            void setRecord(const QString recordId);
            void setShowComponentNames(bool showComponentNames);
            void updateFieldValue(int i);

        signals:
            void recordLinkActivated(const QString& recordId);

        private:
            FieldDefinitionsController& fieldDefinitionsController;
            RecordsController& recordsController;
            TypesController& typesController;

            QString recordId;
            bool showComponentNames;

            const QString getFieldKeyString(const FieldDefinition& field);

        private slots:
            void onRecordLinkActivated(const QString& recordId);
    };
}

#endif // RECORDFIELDSTABLEWIDGET_H
