#ifndef COMPONENTSET_H
#define COMPONENTSET_H

#include "componentlist.h"

namespace Tome
{
    /**
     * @brief Named set of Tome components.
     */
    class ComponentSet
    {
        public:
            /**
             * @brief Name of this component set.
             */
            QString name;

            /**
             * @brief Components of this component set.
             */
            ComponentList components;
    };
}

#endif // COMPONENTSET_H
