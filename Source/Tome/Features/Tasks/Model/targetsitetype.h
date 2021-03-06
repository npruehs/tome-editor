#ifndef TARGETSITETYPE
#define TARGETSITETYPE

#include <QString>

namespace Tome
{
    namespace TargetSiteType
    {
        enum TargetSiteType
        {
            None,
            Record,
            Field,
            Type,
            Component
        };

        inline const QString toString(TargetSiteType targetSiteType)
        {
            switch (targetSiteType)
            {
                case TargetSiteType::Record:
                    return "Record";

                case TargetSiteType::Field:
                    return "Field";

                case TargetSiteType::Type:
                    return "Type";

                case TargetSiteType::Component:
                    return "Component";

                default:
                    return "None";
            }
        }
    }
}

#endif // TARGETSITETYPE
