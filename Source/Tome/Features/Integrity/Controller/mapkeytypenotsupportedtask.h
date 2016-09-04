#ifndef MAPKEYTYPENOTSUPPORTEDTASK_H
#define MAPKEYTYPENOTSUPPORTEDTASK_H

#include "../../Tasks/Controller/task.h"

namespace Tome
{
    class TaskContext;

    class MapKeyTypeNotSupportedTask : public Task
    {
        public:
            MapKeyTypeNotSupportedTask();

            const MessageList execute(const TaskContext& context) const;

            static const QString MessageCode;
    };
}

#endif // MAPKEYTYPENOTSUPPORTEDTASK_H
