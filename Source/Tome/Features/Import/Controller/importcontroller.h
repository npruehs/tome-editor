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
    class TypesController;

    class ImportController : public QObject
    {
            Q_OBJECT

        public:
            ImportController(FieldDefinitionsController& fieldDefinitionsController, RecordsController& recordsController, TypesController& typesController);

            void addRecordImportTemplate(const RecordTableImportTemplate& importTemplate);

            const RecordTableImportTemplate getRecordTableImportTemplate(const QString& name) const;
            const RecordTableImportTemplateList getRecordTableImportTemplates() const;

            void importRecords(const RecordTableImportTemplate& importTemplate, const QVariant& context);

            void removeImportTemplate(const QString& name);

            void setRecordTableImportTemplates(RecordTableImportTemplateList& importTemplates);

        signals:
            void importError(const QString& error) const;
            void importFinished() const;
            void importStarted() const;
            void importTemplatesChanged();
            void progressChanged(const QString title, const QString text, const int currentValue, const int maximumValue) const;

        private slots:
            void onDataAvailable(const QString& importTemplateName, const QVariant& context, const QMap<QString, RecordFieldValueMap>& data) const;
            void onDataUnavailable(const QString& importTemplateName, const QVariant& context, const QString& error) const;
            void onProgressChanged(const QString title, const QString text, const int currentValue, const int maximumValue) const;

        private:
            FieldDefinitionsController& fieldDefinitionsController;
            RecordsController& recordsController;
            TypesController& typesController;

            RecordTableImportTemplateList* model;
    };
}

#endif // IMPORTCONTROLLER_H
