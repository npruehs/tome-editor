#ifndef FINDRECORDCONTROLLER_H
#define FINDRECORDCONTROLLER_H

#include <QObject>

#include "../Model/searchresultlist.h"

namespace Tome
{
    class RecordsController;

    class FindRecordController : public QObject
    {
        Q_OBJECT

        public:
            FindRecordController(const RecordsController& recordsController);

            const SearchResultList findRecord(const QString& searchPattern);

        signals:
            void searchResultChanged(const QString& title, const Tome::SearchResultList results);

        private:
            const RecordsController& recordsController;
    };
}

#endif // FINDRECORDCONTROLLER_H
