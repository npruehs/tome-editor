#ifndef CSVRECORDDATASOURCE_H
#define CSVRECORDDATASOURCE_H

#include "recorddatasource.h"

namespace Tome
{
    class CsvRecordDataSource : public RecordDataSource
    {
        public:
            CsvRecordDataSource();

            const QMap<QString, RecordFieldValueMap> importData(const RecordTableImportTemplate& importTemplate, const QVariant& context) const;
    };
}


#endif // CSVRECORDDATASOURCE_H
