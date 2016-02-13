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
            QString fieldValueDelimiter;
            QString fieldValueTemplate;
            QString fileExtension;
            QString name;
            QString recordDelimiter;
            QString recordFileTemplate;
            QString recordTemplate;
            QMap<QString, QString> typeMap;
    };
}

#endif // RECORDEXPORTTEMPLATE_H
