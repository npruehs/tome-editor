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

            void importData(const RecordTableImportTemplate& importTemplate, const QVariant& context) Q_DECL_OVERRIDE;

        signals:
            void dataAvailable(const QString& importTemplateName, const QVariant& context, const QMap<QString, RecordFieldValueMap>& data) const Q_DECL_OVERRIDE;
            void dataUnavailable(const QString& importTemplateName, const QVariant& context, const QString& error) const Q_DECL_OVERRIDE;
            void progressChanged(const QString title, const QString text, const int currentValue, const int maximumValue) const Q_DECL_OVERRIDE;

        private slots:
            void onFinished(QNetworkReply* reply);

        private:
            QNetworkAccessManager* manager;

            QVariant context;
            QString idColumn;
            QStringList ignoredIds;
            QString importTemplateName;
    };
}

#endif // GOOGLESHEETSRECORDDATASOURCE_H
