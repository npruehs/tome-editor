#ifndef TOMEPROJECTSERIALIZER_H
#define TOMEPROJECTSERIALIZER_H

#include <QIODevice>
#include <QSharedPointer>

#include "../tomeproject.h"

namespace Tome
{
    class TomeProjectSerializer
    {
    public:
        TomeProjectSerializer();

        /**
         * @brief serialize Writes the passed project to the specified device.
         * @param device Device to write the project to.
         * @param project Project to write.
         */
        serialize(QSharedPointer<QIODevice> device, QSharedPointer<TomeProject> project);
    };
}

#endif // TOMEPROJECTSERIALIZER_H
