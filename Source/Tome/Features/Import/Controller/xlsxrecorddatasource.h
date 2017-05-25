#ifndef XLSXRECORDDATASOURCE_H
#define XLSXRECORDDATASOURCE_H

#include "recorddatasource.h"

namespace Tome
{
    /**
     * @brief Data source for importing records from an Excel file.
     */
    class XlsxRecordDataSource : public RecordDataSource
    {
            Q_OBJECT

        public:
            /**
             * @brief Constructs a new data source for importing records from an Excel file.
             */
            XlsxRecordDataSource();

            /**
             * @brief Begins importing records asynchronously using the specified template.
             * @param importTemplate Template to use for importing the record data.
             * @param context XLSX file name.
             */
            void importData(const RecordTableImportTemplate& importTemplate, const QVariant& context) Q_DECL_OVERRIDE;

        signals:
            /**
             * @brief Importing records asynchronously has finished.
             * @param importTemplateName Name of the template that was used for importing the record data.
             * @param context XLSX file name.
             * @param data Imported records and their field values.
             */
            void dataAvailable(const QString& importTemplateName, const QVariant& context, const QMap<QString, RecordFieldValueMap>& data) const Q_DECL_OVERRIDE;

            /**
             * @brief Importing records asynchronously has failed.
             * @param importTemplateName Name of the template that was used for importing the record data.
             * @param context XSLX file name.
             * @param error Localized error message.
             */
            void dataUnavailable(const QString& importTemplateName, const QVariant& context, const QString& error) const Q_DECL_OVERRIDE;

            /**
             * @brief Importing records asynchronously has made notable progress.
             * @param title Localized title of the progress message.
             * @param text Localized text of the progress message.
             * @param currentValue Current progress value (e.g. imported record count).
             * @param maximumValue Maximum progress value (e.g. maximum record count).
             */
            void progressChanged(const QString title, const QString text, const int currentValue, const int maximumValue) const Q_DECL_OVERRIDE;

        private:
            static const QString ParameterSheet;
    };
}

#endif // XLSXRECORDDATASOURCE_H
