#ifndef RECORD_H
#define RECORD_H

#include <QMap>
#include <QString>


namespace Tome
{
    class Record
    {
        public:
            Record();

            QString displayName;
            QString id;
            QMap<QString, QString> fieldValues;
    };
}

#endif // RECORD_H
