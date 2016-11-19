#ifndef MAPVALUETYPEDOESNOTEXISTTASK_H
#define MAPVALUETYPEDOESNOTEXISTTASK_H

#include <QObject>

#include "../../Tasks/Controller/task.h"

namespace Tome
{
    class TaskContext;

    class MapValueTypeDoesNotExistTask : public QObject, public Task
    {
        public:
            MapValueTypeDoesNotExistTask();

            const MessageList execute(const TaskContext& context) const;

            static const QString MessageCode;
    };
}

#endif // MAPVALUETYPEDOESNOTEXISTTASK_H
