#ifndef EXPORTCONTROLLER_H
#define EXPORTCONTROLLER_H

#include <QIODevice>
#include <QString>

#include "../Model/recordexporttemplatelist.h"
#include "../Model/recordexporttemplatemap.h"

namespace Tome
{
    class FieldDefinitionsController;
    class RecordsController;
    class TypesController;

    class ExportController : public QObject
    {
            Q_OBJECT

        public:
            ExportController(const FieldDefinitionsController& fieldDefinitionsController, const RecordsController& recordsController, const TypesController& typesController);

            void addRecordExportTemplate(const RecordExportTemplate& exportTemplate);

            const RecordExportTemplate getRecordExportTemplate(const QString& name) const;
            const RecordExportTemplateMap& getRecordExportTemplates() const;

            bool hasRecordExportTemplate(const QString& name) const;

            void exportRecords(const RecordExportTemplate& exportTemplate, const QString& filePath);
            void exportRecords(const RecordExportTemplate& exportTemplate, QIODevice& device);
            void removeExportTemplate(const QString& name);
            void setRecordExportTemplates(const RecordExportTemplateList& exportTemplates);

        signals:
            void exportTemplatesChanged();

        private:
            RecordExportTemplateMap model;

            static const QString PlaceholderComponents;
            static const QString PlaceholderComponentName;
            static const QString PlaceholderItemType;
            static const QString PlaceholderFieldComponent;
            static const QString PlaceholderFieldDescription;
            static const QString PlaceholderFieldDisplayName;
            static const QString PlaceholderFieldId;
            static const QString PlaceholderFieldKey;
            static const QString PlaceholderFieldType;
            static const QString PlaceholderFieldValue;
            static const QString PlaceholderKeyType;
            static const QString PlaceholderListItem;
            static const QString PlaceholderRecordDisplayName;
            static const QString PlaceholderRecordFields;
            static const QString PlaceholderRecordId;
            static const QString PlaceholderRecordParentId;
            static const QString PlaceholderRecords;
            static const QString PlaceholderValueType;

            const FieldDefinitionsController& fieldDefinitionsController;
            const RecordsController& recordsController;
            const TypesController& typesController;
    };
}

#endif // EXPORTCONTROLLER_H
