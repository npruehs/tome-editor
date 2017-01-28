#ifndef TYPEISNEVERUSEDTASK_H
#define TYPEISNEVERUSEDTASK_H

#include <QObject>

#include "../../Tasks/Controller/task.h"

namespace Tome
{
    class TaskContext;

    class TypeIsNeverUsedTask : public QObject, public Task
    {
        public:
            TypeIsNeverUsedTask();

            const QString getDisplayName() const;
            const MessageList execute(const TaskContext& context) const;

            static const QString MessageCode;
    };
}

#endif // TYPEISNEVERUSEDTASK_H
