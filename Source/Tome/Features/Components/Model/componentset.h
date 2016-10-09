#ifndef COMPONENTSET_H
#define COMPONENTSET_H

#include "componentlist.h"

namespace Tome
{
    class ComponentSet
    {
        public:
            ComponentSet();

            QString name;
            ComponentList components;
    };
}

#endif // COMPONENTSET_H
