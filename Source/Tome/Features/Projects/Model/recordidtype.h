#ifndef RECORDIDTYPE_H
#define RECORDIDTYPE_H

#include <QString>

namespace Tome
{
    namespace RecordIdType
    {
        enum RecordIdType
        {
            Invalid,
            String,
            Integer,
            Uuid
        };

        inline const QString toString(RecordIdType recordIdType)
        {
            switch (recordIdType)
            {
                case RecordIdType::Invalid:
                    return "Invalid";

                case RecordIdType::String:
                    return "String";

                case RecordIdType::Integer:
                    return "Integer";

                case RecordIdType::Uuid:
                    return "Uuid";
            }

            return QString();
        }

        inline RecordIdType fromString(QString recordIdType)
        {
            if (recordIdType == "String")
            {
                return RecordIdType::String;
            }
            else if (recordIdType == "Integer")
            {
                return RecordIdType::Integer;
            }
            else if (recordIdType == "Uuid")
            {
                return RecordIdType::Uuid;
            }

            return RecordIdType::Invalid;
        }
    }
}

#endif // RECORDIDTYPE_H
