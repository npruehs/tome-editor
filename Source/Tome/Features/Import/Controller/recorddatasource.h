#ifndef RECORDDATASOURCE_H
#define RECORDDATASOURCE_H

#include <QMap>

#include "../Model/recordtableimporttemplate.h"
#include "../../Records/Model/recordfieldvaluemap.h"

namespace Tome
{
    /**
     * @brief Data source to import records from.
     */
    class RecordDataSource : public QObject
    {
            Q_OBJECT

        public:
            /**
             * @brief Constructs a new data source to import records from.
             */
            RecordDataSource();
            virtual ~RecordDataSource();

            /**
             * @brief Begins importing records asynchronously using the specified template.
             * @param importTemplate Template to use for importing the record data.
             * @param context Context to import the data in (e.g. source file name).
             */
            virtual void importData(const RecordTableImportTemplate& importTemplate, const QVariant& context) = 0;

            /**
             * @brief Virtual signal emitted as soon as importing records asynchronously has finished.
             * @param importTemplateName Name of the template that was used for importing the record data.
             * @param context Context the data was imported in (e.g. source file name).
             * @param data Imported records and their field values.
             */
            virtual void dataAvailable(const QString& importTemplateName, const QVariant& context, const QMap<QString, RecordFieldValueMap>& data) const = 0;

            /**
             * @brief Virtual signal emitted if importing records asynchronously has failed.
             * @param importTemplateName Name of the template that was used for importing the record data.
             * @param context Context the data was imported in (e.g. source file name).
             * @param error Localized error message.
             */
            virtual void dataUnavailable(const QString& importTemplateName, const QVariant& context, const QString& error) const = 0;

            /**
             * @brief Virtual signal emitted whenever importing records asynchronously has made notable progress.
             * @param title Localized title of the progress message.
             * @param text Localized text of the progress message.
             * @param currentValue Current progress value (e.g. imported record count).
             * @param maximumValue Maximum progress value (e.g. maximum record count).
             */
            virtual void progressChanged(const QString title, const QString text, const int currentValue, const int maximumValue) const = 0;
    };
}

#endif // RECORDDATASOURCE_H
