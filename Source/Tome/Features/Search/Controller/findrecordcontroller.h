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

            const SearchResultList findRecord(const QString& searchPattern) const;

        signals:
            void progressChanged(const QString title, const QString text, const int currentValue, const int maximumValue) const;
            void searchResultChanged(const QString& title, const Tome::SearchResultList results) const;

        private:
            const RecordsController& recordsController;
    };
}

#endif // FINDRECORDCONTROLLER_H
