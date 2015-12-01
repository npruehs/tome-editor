#ifndef PROJECTSERIALIZER_H
#define PROJECTSERIALIZER_H

#include <QIODevice>
#include <QSharedPointer>

#include "project.h"

namespace Tome
{
    class ProjectSerializer
    {
    public:
        ProjectSerializer();

        /**
         * @brief serialize Writes the passed project to the specified device.
         * @param device Device to write the project to.
         * @param project Project to write.
         */
        void serialize(QSharedPointer<QIODevice> device, QSharedPointer<Project> project);
    };
}

#endif // PROJECTSERIALIZER_H
