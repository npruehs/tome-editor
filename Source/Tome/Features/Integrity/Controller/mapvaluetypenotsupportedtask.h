#ifndef MAPVALUETYPENOTSUPPORTEDTASK_H
#define MAPVALUETYPENOTSUPPORTEDTASK_H

#include "../../Tasks/Controller/task.h"

namespace Tome
{
    class TaskContext;

    class MapValueTypeNotSupportedTask : public Task
    {
        public:
            MapValueTypeNotSupportedTask();

            const MessageList execute(const TaskContext& context) const;

            static const QString MessageCode;
    };
}

#endif // MAPVALUETYPENOTSUPPORTEDTASK_H
