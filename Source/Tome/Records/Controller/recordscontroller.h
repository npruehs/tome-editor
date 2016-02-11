#ifndef RECORDSCONTROLLER_H
#define RECORDSCONTROLLER_H

#include <QStringList>

#include "../Model/recordsetlist.h"
#include "../../Fields/Controller/fielddefinitionscontroller.h"

namespace Tome
{
    class RecordsController
    {
        public:
            RecordsController(const FieldDefinitionsController& fieldDefinitionsController);

            void addRecordField(const QString& recordId, const QString& fieldId);
            const RecordSetList& getRecordSets() const;

            /**
             * @brief getRecordByDisplayName Returns the record with the specified display name.
             * @param displayName Display name of the record to get.
             * @return Record with the specified display name.
             */
            const Record& getRecordByDisplayName(const QString& displayName) const;

            /**
             * @brief getRecordNames Returns the list of the names of all records of this project.
             * @return List containing the names of all records of this project.
             */
            const QStringList getRecordNames() const;

            void removeRecordField(const QString& recordId, const QString& fieldId);
            void setRecordSets(RecordSetList& model);
            void updateRecord(const QString& oldId, const QString& newId, const QString& displayName);
            void updateRecordFieldValue(const QString& recordId, const QString& fieldId, const QString& fieldValue);

        private:
            RecordSetList* model;

            const FieldDefinitionsController& fieldDefinitionsController;

            Record* getRecordById(const QString& id) const;
    };
}

#endif // RECORDSCONTROLLER_H
