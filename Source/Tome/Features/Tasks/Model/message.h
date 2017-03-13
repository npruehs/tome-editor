#ifndef MESSAGE_H
#define MESSAGE_H

#include <QVariant>

#include "severity.h"
#include "targetsitetype.h"

namespace Tome
{
    class Message
    {
        public:
            Message();

            QString content;
            QString helpLink;
            QString messageCode;
            Severity::Severity severity;
            QVariant targetSiteId;
            TargetSiteType::TargetSiteType targetSiteType;
    };
}

#endif // MESSAGE_H
