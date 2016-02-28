#ifndef RECORDREFERENCEDOESNOTEXISTTASK_H
#define RECORDREFERENCEDOESNOTEXISTTASK_H

#include <QString>

#include "../../Tasks/Controller/task.h"
#include "../../Tasks/Model/messagelist.h"

namespace Tome
{
    class TaskContext;

    class RecordReferenceDoesNotExistTask : public Task
    {
        public:
            RecordReferenceDoesNotExistTask();

            const QString getId() const;
            const MessageList execute(const TaskContext& context) const;
    };
}

#endif // RECORDREFERENCEDOESNOTEXISTTASK_H
