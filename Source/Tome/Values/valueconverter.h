#ifndef VALUECONVERTER_H
#define VALUECONVERTER_H

#include <QString>

namespace Tome
{
    class ValueConverter
    {
        public:
            ValueConverter();

            QString BoolToString(const bool b) const;
            bool StringToBool(const QString& s) const;
    };
}

#endif // VALUECONVERTER_H
