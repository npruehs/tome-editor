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

            /**
             * @brief serialize Reads the passed record set from the specified device.
             * @param device Device to read the record set from.
             * @param recordSet Record set to fill.
             */
            void deserialize(QSharedPointer<QIODevice> device, QSharedPointer<RecordSet> recordSet) const;

        private:
            static const QString ElementDisplayName;
            static const QString ElementId;
            static const QString ElementRecord;
            static const QString ElementRecords;
            static const QString ElementValue;
    };
}

#endif // RECORDSETSERIALIZER_H