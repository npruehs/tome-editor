#ifndef LISTITEMTYPEDOESNOTEXISTTASK_H
#define LISTITEMTYPEDOESNOTEXISTTASK_H

#include "../../Tasks/Controller/task.h"

namespace Tome
{
    class TaskContext;

    class ListItemTypeDoesNotExistTask : public Task
    {
        public:
            ListItemTypeDoesNotExistTask();

            const QString getId() const;
            const MessageList execute(const TaskContext& context) const;
    };
}

#endif // LISTITEMTYPEDOESNOTEXISTTASK_H
