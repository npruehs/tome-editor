#ifndef RECORDEXPORTER_H
#define RECORDEXPORTER_H

#include <QIODevice>
#include <QSharedPointer>
#include <QString>

#include "recordexporttemplate.h"

#include "../Projects/project.h"


namespace Tome
{
    class RecordExporter
    {
        public:
            RecordExporter();

            void exportRecords(QSharedPointer<QIODevice> device, QSharedPointer<Project> project, QSharedPointer<RecordExportTemplate> exportTemplate);

        private:
            static const QString PlaceholderComponents;
            static const QString PlaceholderComponentName;
            static const QString PlaceholderFieldId;
            static const QString PlaceholderFieldType;
            static const QString PlaceholderFieldValue;
            static const QString PlaceholderRecordFields;
            static const QString PlaceholderRecordId;
            static const QString PlaceholderRecords;
    };
}

#endif // RECORDEXPORTER_H
