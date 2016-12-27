#ifndef RECORDEXPORTTEMPLATE_H
#define RECORDEXPORTTEMPLATE_H

#include <QMap>
#include <QString>

namespace Tome
{
    class RecordExportTemplate
    {
        public:
            RecordExportTemplate();

            QString componentDelimiter;
            QString componentTemplate;

            bool exportAsTable;

            bool exportRoots;
            bool exportInnerNodes;
            bool exportLeafs;

            bool exportLocalizedFieldsOnly;

            QString fieldValueDelimiter;
            QString fieldValueTemplate;
            QString fileExtension;
            QString listTemplate;
            QString listItemTemplate;
            QString listItemDelimiter;
            QString localizedFieldValueTemplate;
            QString mapTemplate;
            QString mapItemTemplate;
            QString mapItemDelimiter;
            QString name;
            QString path;
            QString recordDelimiter;
            QString recordFileTemplate;
            QString recordTemplate;
            QMap<QString, QString> typeMap;

            QStringList ignoredRecords;
            QStringList ignoredFields;
    };
}

#endif // RECORDEXPORTTEMPLATE_H
