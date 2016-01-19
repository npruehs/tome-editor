#ifndef RECORDSET_H
#define RECORDSET_H

#include <QSharedPointer>
#include <QVector>

#include "record.h"

namespace Tome
{
    class RecordSet
    {
        public:
            RecordSet();

            QString name;
            QVector< QSharedPointer<Record> > records;
    };
}

#endif // RECORDSET_H
