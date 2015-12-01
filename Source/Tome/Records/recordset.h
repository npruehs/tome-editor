#ifndef RECORDSET_H
#define RECORDSET_H

#include <list>
#include <QSharedPointer>

#include "record.h"

namespace Tome
{
    class RecordSet
    {
    public:
        RecordSet();

        QString name;
        std::list< QSharedPointer<Record> > records;
    };
}

#endif // RECORDSET_H
