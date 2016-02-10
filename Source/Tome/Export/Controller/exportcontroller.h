#ifndef RECORDEXPORTER_H
#define RECORDEXPORTER_H

#include <QIODevice>
#include <QSharedPointer>
#include <QString>

#include "../Model/recordexporttemplate.h"
#include "../../Fields//Controller/fielddefinitionscontroller.h"
#include "../../Records/Controller/recordscontroller.h"


namespace Tome
{
    class ExportController
    {
        public:
            ExportController(const FieldDefinitionsController& fieldDefinitionsController, const RecordsController& recordsController);

            void exportRecords(const RecordExportTemplate& exportTemplate, QIODevice& device);

        private:
            static const QString PlaceholderComponents;
            static const QString PlaceholderComponentName;
            static const QString PlaceholderFieldId;
            static const QString PlaceholderFieldType;
            static const QString PlaceholderFieldValue;
            static const QString PlaceholderRecordFields;
            static const QString PlaceholderRecordId;
            static const QString PlaceholderRecords;

            const FieldDefinitionsController& fieldDefinitionsController;
            const RecordsController& recordsController;
    };
}

#endif // RECORDEXPORTER_H
