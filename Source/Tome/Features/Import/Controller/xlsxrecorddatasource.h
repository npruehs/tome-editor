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

            void importData(const RecordTableImportTemplate& importTemplate, const QVariant& context) Q_DECL_OVERRIDE;

        signals:
            void dataAvailable(const QString& importTemplateName, const QVariant& context, const QMap<QString, RecordFieldValueMap>& data) const Q_DECL_OVERRIDE;
            void dataUnavailable(const QString& importTemplateName, const QVariant& context, const QString& error) const Q_DECL_OVERRIDE;
            void progressChanged(const QString title, const QString text, const int currentValue, const int maximumValue) const Q_DECL_OVERRIDE;

        private:
            static const QString ParameterSheet;
    };
}

#endif // XLSXRECORDDATASOURCE_H
