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

    class ExportController : public QObject
    {
            Q_OBJECT

        public:
            ExportController(const FacetsController& facetsController,
                             const FieldDefinitionsController& fieldDefinitionsController,
                             const RecordsController& recordsController,
                             const TypesController& typesController);

            void addRecordExportTemplate(const RecordExportTemplate& exportTemplate);

            const RecordExportTemplate getRecordExportTemplate(const QString& name) const;
            const RecordExportTemplateList getRecordExportTemplates() const;

            bool hasRecordExportTemplate(const QString& name) const;

            void exportRecords(const RecordExportTemplate& exportTemplate, const QString& filePath) const;
            void exportRecords(const RecordExportTemplate& exportTemplate, QIODevice& device) const;
            void removeExportTemplate(const QString& name);
            void setRecordExportTemplates(RecordExportTemplateList& exportTemplates);

        signals:
            void exportTemplatesChanged();
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
