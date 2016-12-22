#ifndef RECORDTABLEIMPORTTEMPLATE_H
#define RECORDTABLEIMPORTTEMPLATE_H

#include <QMap>
#include <QString>
#include <QStringList>

#include "tabletype.h"

namespace Tome
{
    class RecordTableImportTemplate
    {
        public:
            RecordTableImportTemplate();

            QString name;
            QString path;
            QString idColumn;
            QStringList ignoredIds;
            QMap<QString, QString> columnMapping;
            TableType::TableType sourceType;
            QString rootRecordId;
            QMap<QString, QString> parameters;
    };
}

#endif // RECORDTABLEIMPORTTEMPLATE_H
