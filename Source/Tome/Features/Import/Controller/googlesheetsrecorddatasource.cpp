#include "googlesheetsrecorddatasource.h"

#include <QNetworkReply>
#include <QTextStream>

using namespace Tome;

GoogleSheetsRecordDataSource::GoogleSheetsRecordDataSource()
    : manager(new QNetworkAccessManager(this))
{
    connect(this->manager,
            SIGNAL(finished(QNetworkReply*)),
            SLOT(onFinished(QNetworkReply*)));
}

void GoogleSheetsRecordDataSource::importData(const RecordTableImportTemplate& importTemplate, const QVariant& context)
{
    this->context = context;
    this->idColumn = importTemplate.idColumn;
    this->ignoredIds = importTemplate.ignoredIds;
    this->importTemplateName = importTemplate.name;

    // Show progress bar.
    QString progressBarTitle = tr("Importing %1 With %2").arg(context.toString(), importTemplate.name);
    emit this->progressChanged(progressBarTitle, tr("Connecting to Google"), 0, 100);

    // Connect to Google.
    QString urlString = QString("https://docs.google.com/spreadsheets/d/%1/export?format=csv").arg(context.toString());
    QUrl url(urlString);
    QNetworkRequest request(url);
    this->manager->get(request);
}

void GoogleSheetsRecordDataSource::onFinished(QNetworkReply* reply)
{
    reply->deleteLater();

    // Check for errors.
    if (reply->error() != QNetworkReply::NoError)
    {
        qCritical(reply->errorString().toUtf8().constData());
        emit this->dataUnavailable(this->importTemplateName, this->context, reply->errorString());
        return;
    }

    int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    if (status < 200 || status >= 300)
    {
        QString errorMessage = QObject::tr("HTTP %1 while accessing the sheet - is link sharing active?")
                .arg(QString::number(status));
        qCritical(errorMessage.toUtf8().constData());
        emit this->dataUnavailable(this->importTemplateName, this->context, errorMessage);
        return;
    }

    QTextStream textStream(reply);
    textStream.setCodec("UTF-8");

    // Read headers.
    QString line;
    line = textStream.readLine();

    if (line.isEmpty())
    {
        QString errorMessage = QObject::tr("Source sheet is empty.");
        qCritical(errorMessage.toUtf8().constData());
        emit this->dataUnavailable(this->importTemplateName, this->context, errorMessage);
        return;
    }

    QStringList headers = line.split(',');

    // Find id column.
    int idColumnIndex = -1;

    for (int i = 0; i < headers.count(); ++i)
    {
        if (headers[i] == this->idColumn)
        {
            idColumnIndex = i;
            break;
        }
    }

    if (idColumnIndex == -1)
    {
        QString errorMessage = QObject::tr("Could not find id column %1 in source sheet.")
                .arg(this->idColumn);
        qCritical(errorMessage.toUtf8().constData());
        emit this->dataUnavailable(this->importTemplateName, this->context, errorMessage);
        return;
    }

    // Read rows.
    QMap<QString, RecordFieldValueMap> data;
    int rowIndex = 0;
    QString progressBarTitle = tr("Importing %1 With %2").arg(this->context.toString(), this->importTemplateName);

    while (!(line = textStream.readLine()).isEmpty())
    {
        ++rowIndex;
        QStringList row = line.split(',');

        if (row.count() != headers.count())
        {
            QString errorMessage = QObject::tr("Row %1 has %2 columns, but the header has %3 columns.")
                    .arg(QString::number(rowIndex), QString::number(row.count()), QString::number(headers.count()));
            qCritical(errorMessage.toUtf8().constData());
            emit this->dataUnavailable(this->importTemplateName, this->context, errorMessage);
            return;
        }

        // Get record id.
        QString recordId = row[idColumnIndex];

        // Update progress bar.
        emit this->progressChanged(progressBarTitle, recordId, reply->pos(), reply->size());

        // Check if ignored.
        if (this->ignoredIds.contains(recordId))
        {
            continue;
        }

        // Get data.
        RecordFieldValueMap map;

        for (int i = 0; i < row.count(); ++i)
        {
            if (i == idColumnIndex)
            {
                continue;
            }

            map[headers[i]] = row[i];
        }

        data[recordId] = map;
    }

    emit this->progressChanged(progressBarTitle, QString(), 1, 1);
    emit this->dataAvailable(this->importTemplateName, this->context, data);
}
