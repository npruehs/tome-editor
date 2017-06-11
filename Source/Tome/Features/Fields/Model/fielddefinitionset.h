#ifndef FIELDDEFINITIONSET_H
#define FIELDDEFINITIONSET_H

#include <QVector>

#include "fielddefinitionlist.h"

namespace Tome
{
    /**
     * @brief Named set of Tome field definitions.
     */
    class FieldDefinitionSet
    {
        public:
            /**
             * @brief Name of this field definition set.
             */
            QString name;

            /**
             * @brief Field definitions of this field definition set.
             */
            FieldDefinitionList fieldDefinitions;
    };
}

#endif // FIELDDEFINITIONSET_H
