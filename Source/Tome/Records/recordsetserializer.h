#ifndef RECORDSETSERIALIZER_H
#define RECORDSETSERIALIZER_H

#include <QIODevice>
#include <QSharedPointer>

#include "recordset.h"

namespace Tome
{
    class RecordSetSerializer
    {
    public:
        RecordSetSerializer();

        /**
         * @brief serialize Writes the passed record set to the specified device.
         * @param device Device to write the record set to.
         * @param recordSet Record set to write.
         */
        void serialize(QSharedPointer<QIODevice> device, QSharedPointer<RecordSet> recordSet) const;
    };
}

#endif // RECORDSETSERIALIZER_H
