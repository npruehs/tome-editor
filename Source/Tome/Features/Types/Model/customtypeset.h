#ifndef CUSTOMTYPESET_H
#define CUSTOMTYPESET_H

#include "customtypelist.h"

namespace Tome
{
    /**
     * @brief Named set of Tome custom types.
     */
    class CustomTypeSet
    {
        public:
            /**
             * @brief Name of this custom type set.
             */
            QString name;

            /**
             * @brief Custom types of this custom type set.
             */
            CustomTypeList types;
    };
}

#endif // CUSTOMTYPESET_H
