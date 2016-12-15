#include "csvrecorddatasource.h"

#include <stdexcept>

#include <QFile>
#include <QTextStream>

using namespace Tome;


CsvRecordDataSource::CsvRecordDataSource()
{
}

const QMap<QString, RecordFieldValueMap> CsvRecordDataSource::importData(const RecordTableImportTemplate& importTemplate, const QVariant& context) const
{
    const QString filePath = context.toString();
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly))
    {
        QString errorMessage = QObject::tr("Source file could not be read:\r\n") + filePath;
        qCritical(errorMessage.toUtf8().constData());
        throw std::runtime_error(errorMessage.toStdString());
    }

    QMap<QString, RecordFieldValueMap> data;

    QTextStream textStream(&file);
    textStream.setCodec("UTF-8");

    QString line;
    line = textStream.readLine();

    if (line.isEmpty())
    {
        QString errorMessage = QObject::tr("Source file is empty:\r\n") + filePath;
        qCritical(errorMessage.toUtf8().constData());
        throw std::runtime_error(errorMessage.toStdString());
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
        qCritical(errorMessage.toUtf8().constData());
        throw std::runtime_error(errorMessage.toStdString());
    }

    int rowIndex = 1;

    while (!(line = textStream.readLine()).isEmpty())
    {
        QStringList row = line.split(';');

        if (row.count() != headers.count())
        {
            QString errorMessage = QObject::tr("Row %1 has %2 columns, but the header has %3 columns.")
                    .arg(rowIndex, row.count(), headers.count());
            qCritical(errorMessage.toUtf8().constData());
            throw std::runtime_error(errorMessage.toStdString());
        }

        RecordFieldValueMap map;

        for (int i = 0; i < row.count(); ++i)
        {
            if (i == idColumnIndex)
            {
                continue;
            }

            map[headers[i]] = row[i];
        }

        data[row[idColumnIndex]] = map;
    }

    return data;
}
