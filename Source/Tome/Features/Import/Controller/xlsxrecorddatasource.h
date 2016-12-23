#ifndef XLSXRECORDDATASOURCE_H
#define XLSXRECORDDATASOURCE_H

#include "recorddatasource.h"

namespace Tome
{
    class XlsxRecordDataSource : public RecordDataSource
    {
            Q_OBJECT

        public:
            XlsxRecordDataSource();

            void importData(const RecordTableImportTemplate& importTemplate, const QVariant& context);

        signals:
            void dataAvailable(const QMap<QString, RecordFieldValueMap>& data) const;
            void dataUnavailable(const QString& error) const;

        private:
            static const QString ParameterSheet;
    };
}

#endif // XLSXRECORDDATASOURCE_H
