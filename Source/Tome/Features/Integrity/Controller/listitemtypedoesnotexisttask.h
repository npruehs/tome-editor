#ifndef LISTITEMTYPEDOESNOTEXISTTASK_H
#define LISTITEMTYPEDOESNOTEXISTTASK_H

#include <QObject>

#include "../../Tasks/Controller/task.h"

namespace Tome
{
    class TaskContext;

    class ListItemTypeDoesNotExistTask : public QObject, public Task
    {
        public:
            ListItemTypeDoesNotExistTask();

            const MessageList execute(const TaskContext& context) const;

            static const QString MessageCode;
    };
}

#endif // LISTITEMTYPEDOESNOTEXISTTASK_H
