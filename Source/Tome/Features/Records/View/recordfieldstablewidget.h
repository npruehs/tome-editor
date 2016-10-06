#ifndef RECORDFIELDSTABLEWIDGET_H
#define RECORDFIELDSTABLEWIDGET_H

#include <QTableWidget>

namespace Tome
{
    class FieldDefinitionsController;
    class RecordsController;
    class TypesController;

    class RecordFieldsTableWidget : public QTableWidget
    {
        Q_OBJECT

        public:
            RecordFieldsTableWidget(FieldDefinitionsController& fieldDefinitionsController, RecordsController& recordsController, TypesController& typesController);

            void setDescriptionColumnEnabled(bool enabled);
            void setRecord(int i, const QString recordId);

        signals:
            void recordLinkActivated(const QString& recordId);

        private:
            FieldDefinitionsController& fieldDefinitionsController;
            RecordsController& recordsController;
            TypesController& typesController;

        private slots:
            void onRecordLinkActivated(const QString& recordId);
    };
}

#endif // RECORDFIELDSTABLEWIDGET_H
