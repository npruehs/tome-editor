#ifndef FIELDDEFINITIONSETSERIALIZER_H
#define FIELDDEFINITIONSETSERIALIZER_H

#include <QIODevice>
#include <QSharedPointer>

#include "fielddefinitionset.h"

namespace Tome
{
    class FieldDefinitionSetSerializer
    {
    public:
        FieldDefinitionSetSerializer();

        /**
         * @brief serialize Writes the passed field definition set to the specified device.
         * @param device Device to write the field definition set to.
         * @param project Field definition set to write.
         */
        void serialize(QSharedPointer<QIODevice> device, QSharedPointer<FieldDefinitionSet> fieldDefinitionSet);
    };
}


#endif // FIELDDEFINITIONSETSERIALIZER_H
