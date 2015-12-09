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
            void serialize(QSharedPointer<QIODevice> device, QSharedPointer<Project> project) const;

            /**
             * @brief deserialize Reads the project from the specified device.
             * @param device Device to read the project from.
             * @param project Project to fill with read data.
             */
            void deserialize(QSharedPointer<QIODevice> device, QSharedPointer<Project> project) const;
    };
}

#endif // PROJECTSERIALIZER_H
