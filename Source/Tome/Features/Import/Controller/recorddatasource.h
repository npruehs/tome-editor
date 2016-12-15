#ifndef RECORDDATASOURCE_H
#define RECORDDATASOURCE_H

#include <QMap>

#include "../Model/recordtableimporttemplate.h"
#include "../../Records/Model/recordfieldvaluemap.h"

namespace Tome
{
    class RecordDataSource
    {
        public:
            RecordDataSource();
            virtual ~RecordDataSource();

            virtual const QMap<QString, RecordFieldValueMap> importData(const RecordTableImportTemplate& importTemplate, const QVariant& context) const = 0;
    };
}

#endif // RECORDDATASOURCE_H
