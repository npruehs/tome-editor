#ifndef RECORDDATASOURCE_H
#define RECORDDATASOURCE_H

#include <QMap>

#include "../Model/recordtableimporttemplate.h"
#include "../../Records/Model/recordfieldvaluemap.h"

namespace Tome
{
    class RecordDataSource : public QObject
    {
            Q_OBJECT

        public:
            RecordDataSource();
            virtual ~RecordDataSource();

            virtual void importData(const RecordTableImportTemplate& importTemplate, const QVariant& context) = 0;

            virtual void dataAvailable(const QString& importTemplateName, const QVariant& context, const QMap<QString, RecordFieldValueMap>& data) const = 0;
            virtual void dataUnavailable(const QString& importTemplateName, const QVariant& context, const QString& error) const = 0;
            virtual void progressChanged(const QString title, const QString text, const int currentValue, const int maximumValue) const = 0;
    };
}

#endif // RECORDDATASOURCE_H
