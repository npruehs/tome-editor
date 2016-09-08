#ifndef MAPKEYTYPEDOESNOTEXISTTASK_H
#define MAPKEYTYPEDOESNOTEXISTTASK_H

#include "../../Tasks/Controller/task.h"

namespace Tome
{
    class TaskContext;

    class MapKeyTypeDoesNotExistTask : public Task
    {
        public:
            MapKeyTypeDoesNotExistTask();

            const MessageList execute(const TaskContext& context) const;

            static const QString MessageCode;
    };
}

#endif // MAPKEYTYPEDOESNOTEXISTTASK_H
