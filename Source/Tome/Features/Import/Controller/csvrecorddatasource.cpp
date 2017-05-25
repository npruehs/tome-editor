#include "csvrecorddatasource.h"

#include <QFile>
#include <QTextStream>

using namespace Tome;


CsvRecordDataSource::CsvRecordDataSource()
{
}

void CsvRecordDataSource::importData(const RecordTableImportTemplate& importTemplate, const QVariant& context)
{
    // Open CSV file.
    const QString filePath = context.toString();
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly))
    {
        QString errorMessage = QObject::tr("Source file could not be read:\r\n") + filePath;
        qCritical(qUtf8Printable(errorMessage));
        emit this->dataUnavailable(importTemplate.name, context, errorMessage);
        return;
    }

    // Show progress bar.
    QString progressBarTitle = tr("Importing %1 With %2").arg(file.fileName(), importTemplate.name);
    emit this->progressChanged(progressBarTitle, tr("Opening File"), 0, 100);

    QTextStream textStream(&file);
    textStream.setCodec("UTF-8");

    // Read headers.
    QString line;
    line = textStream.readLine();

    if (line.isEmpty())
    {
        QString errorMessage = QObject::tr("Source file is empty:\r\n") + filePath;
        qCritical(qUtf8Printable(errorMessage));
        file.close();
        emit this->dataUnavailable(importTemplate.name, context, errorMessage);
        return;
    }

    QStringList headers = line.split(';');

    // Find id column.
    int idColumnIndex = -1;

    for (int i = 0; i < headers.count(); ++i)
    {
        if (headers[i] == importTemplate.idColumn)
        {
            idColumnIndex = i;
            break;
        }
    }

    if (idColumnIndex == -1)
    {
        QString errorMessage = QObject::tr("Could not find id column %1 in source file:\r\n%2")
                .arg(importTemplate.idColumn, filePath);
        qCritical(qUtf8Printable(errorMessage));
        file.close();
        emit this->dataUnavailable(importTemplate.name, context, errorMessage);
        return;
    }

    // Read rows.
    QMap<QString, RecordFieldValueMap> data;
    int rowIndex = 0;

    while (!(line = textStream.readLine()).isEmpty())
    {
        ++rowIndex;
        QStringList row = line.split(';');

        if (row.count() != headers.count())
        {
            QString errorMessage = QObject::tr("Row %1 has %2 columns, but the header has %3 columns.")
                    .arg(QString::number(rowIndex), QString::number(row.count()), QString::number(headers.count()));
            qCritical(qUtf8Printable(errorMessage));
            file.close();
            emit this->dataUnavailable(importTemplate.name, context, errorMessage);
            return;
        }

        // Get record id.
        QString recordId = row[idColumnIndex];

        // Update progress bar.
        emit this->progressChanged(progressBarTitle, recordId, file.pos(), file.size());

        // Check if ignored.
        if (importTemplate.ignoredIds.contains(recordId))
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

    file.close();

    emit this->progressChanged(progressBarTitle, QString(), 1, 1);
    emit this->dataAvailable(importTemplate.name, context, data);
}
