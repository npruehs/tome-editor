#ifndef SEARCHRESULT_H
#define SEARCHRESULT_H

#include <QString>

#include "../../Tasks/Model/targetsitetype.h"

namespace Tome
{
    class SearchResult
    {
        public:
            SearchResult();

            QString content;
            QString targetSiteId;
            TargetSiteType::TargetSiteType targetSiteType;
    };
}

#endif // SEARCHRESULT_H
