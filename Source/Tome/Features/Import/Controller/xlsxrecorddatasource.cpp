#include "xlsxrecorddatasource.h"

#include <QFileInfo>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

using namespace Tome;


const QString XlsxRecordDataSource::ParameterSheet = "Sheet";


XlsxRecordDataSource::XlsxRecordDataSource()
{
}

void XlsxRecordDataSource::importData(const RecordTableImportTemplate& importTemplate, const QVariant& context)
{
    // Open Excel file.
    const QString filePath = context.toString();
    const QFileInfo fileInfo = QFileInfo(filePath);

    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("DRIVER={Microsoft Excel Driver (*.xls, *.xlsx, *.xlsm, *.xlsb)};DBQ=" + filePath);

    if (!db.open())
    {
        QString errorMessage =
                QObject::tr("Source file could not be read:\r\n") + filePath
                + "\r\n\r\n" + db.lastError().text();

        qCritical(errorMessage.toUtf8().constData());
        emit this->dataUnavailable(importTemplate.name, context, errorMessage);
        return;
    }

    // Show progress bar.
    QString progressBarTitle = tr("Importing %1 With %2").arg(fileInfo.fileName(), importTemplate.name);
    emit this->progressChanged(progressBarTitle, tr("Opening File"), 0, 100);

    // Read headers.
    if (!importTemplate.parameters.contains(ParameterSheet))
    {
        QString errorMessage =
                QObject::tr("Parameter missing:\r\n%1\r\n\r\nIn import template:\r\n%2")
                .arg(ParameterSheet, filePath);

        qCritical(errorMessage.toUtf8().constData());
        emit this->dataUnavailable(importTemplate.name, context, errorMessage);
        return;
    }

    QString sheet = importTemplate.parameters[ParameterSheet];
    QSqlQuery sqlQuery(QString("SELECT * FROM [%1$]").arg(sheet));
    QSqlRecord sqlRecord = sqlQuery.record();
    int columns = sqlRecord.count();

    QStringList headers;

    for (int i = 0; i < columns; ++i)
    {
        headers << sqlRecord.fieldName(i);
    }

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
        emit this->dataUnavailable(importTemplate.name, context, errorMessage);
        return;
    }

    // Read rows.
    QMap<QString, RecordFieldValueMap> data;
    int rowIndex = 0;

    while (sqlQuery.next())
    {
        ++rowIndex;

        RecordFieldValueMap map;
        QString recordId;

        for (int i = 0; i < columns; ++i)
        {
            QVariant value = sqlQuery.value(i).toString();

            if (i == idColumnIndex)
            {
                recordId = value.toString();
            }
            else
            {
                map[headers[i]] = value;
            }
        }

        // Update progress bar.
        emit this->progressChanged(progressBarTitle, recordId, rowIndex, sqlQuery.size());

        if (importTemplate.ignoredIds.contains(recordId))
        {
            continue;
        }

        data[recordId] = map;
    }

    emit this->progressChanged(progressBarTitle, QString(), 1, 1);
    emit this->dataAvailable(importTemplate.name, context, data);
}
