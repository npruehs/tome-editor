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
            IncrementingInteger,
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
                case RecordIdType::IncrementingInteger:
                    return "IncrementingInteger";
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
            }else if(recordIdType == "IncrementingInteger"){
                return RecordIdType::IncrementingInteger;
            }

            return RecordIdType::Invalid;
        }
    }
}

#endif // RECORDIDTYPE_H
