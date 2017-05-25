#ifndef RECORDSET_H
#define RECORDSET_H

#include "recordlist.h"

namespace Tome
{
    /**
     * @brief Named set of Tome records.
     */
    class RecordSet
    {
        public:
            /**
             * @brief Name of this record set.
             */
            QString name;

            /**
             * @brief Records of this record set.
             */
            RecordList records;
    };
}

#endif // RECORDSET_H
