#include "xlsxrecorddatasource.h"

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

    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("DRIVER={Microsoft Excel Driver (*.xls, *.xlsx, *.xlsm, *.xlsb)};DBQ=" + filePath);

    if (!db.open())
    {
        QString errorMessage =
                QObject::tr("Source file could not be read:\r\n") + filePath
                + "\r\n\r\n" + db.lastError().text();

        qCritical(errorMessage.toUtf8().constData());
        emit this->dataUnavailable(errorMessage);
        return;
    }

    // Read headers.
    if (!importTemplate.parameters.contains(ParameterSheet))
    {
        QString errorMessage =
                QObject::tr("Parameter missing:\r\n%1\r\n\r\nIn import template:\r\n%2")
                .arg(ParameterSheet, filePath);

        qCritical(errorMessage.toUtf8().constData());
        emit this->dataUnavailable(errorMessage);
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
        emit this->dataUnavailable(errorMessage);
        return;
    }

    // Read rows.
    QMap<QString, RecordFieldValueMap> data;

    while (sqlQuery.next())
    {
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

        if (importTemplate.ignoredIds.contains(recordId))
        {
            continue;
        }

        data[recordId] = map;
    }

    emit this->dataAvailable(data);
}
