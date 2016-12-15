#ifndef IMPORTCONTROLLER_H
#define IMPORTCONTROLLER_H

#include <QString>
#include <QVariant>

#include "../Model/recordtableimporttemplatelist.h"

namespace Tome
{
    class FieldDefinitionsController;
    class RecordsController;

    class ImportController : public QObject
    {
            Q_OBJECT

        public:
            ImportController(FieldDefinitionsController& fieldDefinitionsController, RecordsController& recordsController);

            const RecordTableImportTemplate getRecordTableImportTemplate(const QString& name) const;
            const RecordTableImportTemplateList getRecordTableImportTemplates() const;

            void importRecords(const RecordTableImportTemplate& importTemplate, const QVariant& context);

            void setRecordTableImportTemplates(RecordTableImportTemplateList& importTemplates);

        private:
            FieldDefinitionsController& fieldDefinitionsController;
            RecordsController& recordsController;

            RecordTableImportTemplateList* model;
    };
}

#endif // IMPORTCONTROLLER_H
