#ifndef SEARCHRESULT_H
#define SEARCHRESULT_H

#include <QVariant>

#include "../../Tasks/Model/targetsitetype.h"

namespace Tome
{
    class SearchResult
    {
        public:
            SearchResult();

            QString content;
            QVariant targetSiteId;
            TargetSiteType::TargetSiteType targetSiteType;
    };
}

#endif // SEARCHRESULT_H
