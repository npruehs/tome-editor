#ifndef FINDRECORDCONTROLLER_H
#define FINDRECORDCONTROLLER_H

#include <QObject>

#include "../Model/searchresultlist.h"

namespace Tome
{
    class RecordsController;

    /**
     * @brief Controller for finding specific records.
     */
    class FindRecordController : public QObject
    {
        Q_OBJECT

        public:
            /**
             * @brief Constructs a new controller for finding specific records.
             * @param recordsController Controller for adding, updating and removing records.
             */
            FindRecordController(const RecordsController& recordsController);

            /**
             * @brief Searches for records whose id or display name contain the specified string.
             * @param searchPattern String to search for.
             * @return Records whose id or display name contain the specified string.
             */
            const SearchResultList findRecord(const QString& searchPattern) const;

        signals:
            /**
             * @brief Progress of the current search operation has changed.
             * @param title Title of the current search operation step.
             * @param text Details of the current search operation step.
             * @param currentValue Current progress value of the current search operation step.
             * @param maximumValue Maximum progress value of the current search operation step.
             */
            void progressChanged(const QString title, const QString text, const int currentValue, const int maximumValue) const;

            /**
             * @brief A search has been completed.
             * @param title Title of the search operation.
             * @param results Results of the search operation.
             */
            void searchResultChanged(const QString& title, const Tome::SearchResultList results) const;

        private:
            const RecordsController& recordsController;
    };
}

#endif // FINDRECORDCONTROLLER_H
