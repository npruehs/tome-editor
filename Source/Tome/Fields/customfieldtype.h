#ifndef CUSTOMFIELDTYPE_H
#define CUSTOMFIELDTYPE_H

#include <QMap>
#include <QString>

namespace Tome
{
    class CustomFieldType
    {
        public:
            CustomFieldType();

            QString name;
            QString baseType;
            QMap<QString, QString> restrictions;
    };
}

#endif // CUSTOMFIELDTYPE_H
