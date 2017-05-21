#ifndef SEARCHRESULT_H
#define SEARCHRESULT_H

#include <QVariant>

#include "../../Tasks/Model/targetsitetype.h"

namespace Tome
{
    /**
     * @brief Result of a search operation.
     */
    class SearchResult
    {
        public:
            /**
             * @brief Actual search result.
             */
            QString content;

            /**
             * @brief Where the search result was found.
             */
            QVariant targetSiteId;

            /**
             * @brief Type of the location where the search result was found.
             */
            TargetSiteType::TargetSiteType targetSiteType;
    };
}

#endif // SEARCHRESULT_H
