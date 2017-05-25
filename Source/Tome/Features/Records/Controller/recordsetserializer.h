#ifndef RECORDSETSERIALIZER_H
#define RECORDSETSERIALIZER_H

#include <QIODevice>

namespace Tome
{
    class RecordSet;

    /**
     * @brief Reads and writes records from any device.
     */
    class RecordSetSerializer : public QObject
    {
            Q_OBJECT

        public:
            /**
             * @brief Writes the passed record set to the specified device.
             * @param device Device to write the record set to.
             * @param recordSet Record set to write.
             */
            void serialize(QIODevice& device, const RecordSet& recordSet) const;

            /**
             * @brief Reads the passed record set from the specified device.
             * @param device Device to read the record set from.
             * @param recordSet Record set to fill.
             */
            void deserialize(QIODevice& device, RecordSet& recordSet) const;

        signals:
            /**
             * @brief Progress of the current record operation has changed.
             * @param title Title of the current record operation step.
             * @param text Details of the current record operation step.
             * @param currentValue Current progress value of the current record operation step.
             * @param maximumValue Maximum progress value of the current record operation step.
             */
            void progressChanged(const QString title, const QString text, const int currentValue, const int maximumValue) const;

        private:
            static const QString ElementDisplayName;
            static const QString ElementEditorIconFieldId;
            static const QString ElementId;
            static const QString ElementItem;
            static const QString ElementKey;
            static const QString ElementParentId;
            static const QString ElementReadOnly;
            static const QString ElementRecord;
            static const QString ElementRecords;
            static const QString ElementValue;
    };
}

#endif // RECORDSETSERIALIZER_H
