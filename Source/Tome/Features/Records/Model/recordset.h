#ifndef RECORDSET_H
#define RECORDSET_H

#include "record.h"
#include "recordlist.h"

namespace Tome
{
    class RecordSet
    {
        public:
            RecordSet();

            QString name;
            RecordList records;
    };
}

#endif // RECORDSET_H
