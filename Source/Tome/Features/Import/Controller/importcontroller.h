#ifndef IMPORTCONTROLLER_H
#define IMPORTCONTROLLER_H

#include <QString>
#include <QVariant>

#include "../Model/recordtableimporttemplatelist.h"
#include "../../Records/Model/recordfieldvaluemap.h"

namespace Tome
{
    class FieldDefinitionsController;
    class RecordsController;

    class ImportController : public QObject
    {
            Q_OBJECT

        public:
            ImportController(FieldDefinitionsController& fieldDefinitionsController, RecordsController& recordsController);

            void addRecordImportTemplate(const RecordTableImportTemplate& importTemplate);

            const RecordTableImportTemplate getRecordTableImportTemplate(const QString& name) const;
            const RecordTableImportTemplateList getRecordTableImportTemplates() const;

            void importRecords(const RecordTableImportTemplate& importTemplate, const QVariant& context);

            void removeImportTemplate(const QString& name);

            void setRecordTableImportTemplates(RecordTableImportTemplateList& importTemplates);

        signals:
            void dataUnavailable(const QString& error) const;
            void importTemplatesChanged();

        private slots:
            void onDataAvailable(const QMap<QString, RecordFieldValueMap>& data) const;
            void onDataUnavailable(const QString& error) const;

        private:
            FieldDefinitionsController& fieldDefinitionsController;
            RecordsController& recordsController;

            RecordTableImportTemplateList* model;
    };
}

#endif // IMPORTCONTROLLER_H
