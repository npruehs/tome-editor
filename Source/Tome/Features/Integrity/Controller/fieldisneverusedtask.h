#ifndef FIELDISNEVERUSEDTASK_H
#define FIELDISNEVERUSEDTASK_H

#include <QObject>

#include "../../Tasks/Controller/task.h"

namespace Tome
{
    class TaskContext;

    class FieldIsNeverUsedTask : public QObject, public Task
    {
        public:
            FieldIsNeverUsedTask();

            const QString getDisplayName() const;
            const MessageList execute(const TaskContext& context) const;

            static const QString MessageCode;
    };
}

#endif // FIELDISNEVERUSEDTASK_H
