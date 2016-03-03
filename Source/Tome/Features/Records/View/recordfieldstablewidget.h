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
        public:
            RecordFieldsTableWidget(FieldDefinitionsController& fieldDefinitionsController, RecordsController& recordsController, TypesController& typesController);

            void setRecord(int i, const QString recordId);

        private:
            FieldDefinitionsController& fieldDefinitionsController;
            RecordsController& recordsController;
            TypesController& typesController;
    };
}

#endif // RECORDFIELDSTABLEWIDGET_H
