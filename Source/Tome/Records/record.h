#ifndef RECORD_H
#define RECORD_H

#include <QString>
#include <map>

namespace Tome
{
    class Record
    {
        public:
            Record();

            QString id;
            std::map<QString, QString> fieldValues;
    };
}

#endif // RECORD_H
