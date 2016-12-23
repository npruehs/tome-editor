#ifndef GOOGLESHEETSRECORDDATASOURCE_H
#define GOOGLESHEETSRECORDDATASOURCE_H

#include <QNetworkAccessManager>

#include "recorddatasource.h"


namespace Tome
{
    class GoogleSheetsRecordDataSource : public RecordDataSource
    {
            Q_OBJECT

        public:
            GoogleSheetsRecordDataSource();

            void importData(const RecordTableImportTemplate& importTemplate, const QVariant& context);

        signals:
            void dataAvailable(const QMap<QString, RecordFieldValueMap>& data) const;
            void dataUnavailable(const QString& error) const;

        private slots:
            void onFinished(QNetworkReply* reply);

        private:
            QNetworkAccessManager* manager;

            QString idColumn;
            QStringList ignoredIds;
    };
}

#endif // GOOGLESHEETSRECORDDATASOURCE_H
