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
             * @brief Constructs a new component set without name or components.
             */
            ComponentSet();

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
