#ifndef FIELDDEFINITIONFILESERIALIZER_H
#define FIELDDEFINITIONFILESERIALIZER_H

#include <QIODevice>
#include <QSharedPointer>

#include "fielddefinitionfile.h"

namespace Tome
{
    class FieldDefinitionFileSerializer
    {
    public:
        FieldDefinitionFileSerializer();

        /**
         * @brief serialize Writes the passed field definition file to the specified device.
         * @param device Device to write the field definition file to.
         * @param project Field definition file to write.
         */
        void serialize(QSharedPointer<QIODevice> device, QSharedPointer<FieldDefinitionFile> fieldDefinitionFile);
    };
}


#endif // FIELDDEFINITIONFILESERIALIZER_H
