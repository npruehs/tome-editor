#ifndef XLSXRECORDDATASOURCE_H
#define XLSXRECORDDATASOURCE_H

#include "recorddatasource.h"

namespace Tome
{
    class XlsxRecordDataSource : public RecordDataSource
    {
        public:
            XlsxRecordDataSource();

            const QMap<QString, RecordFieldValueMap> importData(const RecordTableImportTemplate& importTemplate, const QVariant& context) const;

        private:
            static const QString ParameterSheet;
    };
}

#endif // XLSXRECORDDATASOURCE_H
