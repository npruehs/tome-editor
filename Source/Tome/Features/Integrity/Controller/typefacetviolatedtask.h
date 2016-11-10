#ifndef TYPEFACETVIOLATEDTASK_H
#define TYPEFACETVIOLATEDTASK_H

#include "../../Tasks/Controller/task.h"

namespace Tome
{
    class TaskContext;

    class TypeFacetViolatedTask : public Task
    {
        public:
            TypeFacetViolatedTask();

            const MessageList execute(const TaskContext& context) const;

            static const QString MessageCode;
    };
}

#endif // TYPEFACETVIOLATEDTASK_H
