#ifndef RECORDSCONTROLLER_H
#define RECORDSCONTROLLER_H

#include "../Model/recordsetlist.h"


namespace Tome
{
    class RecordsController
    {
        public:
            RecordsController();

            const RecordSetList& getRecordSets() const;
            void setRecordSets(RecordSetList& model);

        private:
            RecordSetList* model;
    };
}

#endif // RECORDSCONTROLLER_H
