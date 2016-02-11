#ifndef RECORDSCONTROLLER_H
#define RECORDSCONTROLLER_H

#include <QStringList>

#include "../Model/recordsetlist.h"


namespace Tome
{
    class RecordsController
    {
        public:
            RecordsController();

            const RecordSetList& getRecordSets() const;

            /**
             * @brief getRecordNames Returns the list of the names of all records of this project.
             * @return List containing the names of all records of this project.
             */
            const QStringList getRecordNames() const;

            void setRecordSets(RecordSetList& model);

        private:
            RecordSetList* model;
    };
}

#endif // RECORDSCONTROLLER_H
