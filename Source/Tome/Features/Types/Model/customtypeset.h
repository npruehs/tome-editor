#ifndef CUSTOMTYPESET_H
#define CUSTOMTYPESET_H

#include "customtypelist.h"

namespace Tome
{
    class CustomTypeSet
    {
        public:
            CustomTypeSet();

            QString name;
            CustomTypeList types;
    };
}

#endif // CUSTOMTYPESET_H
