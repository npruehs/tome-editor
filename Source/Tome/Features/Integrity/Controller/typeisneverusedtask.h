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

            const QString getDisplayName() const Q_DECL_OVERRIDE;
            const MessageList execute(const TaskContext& context) const Q_DECL_OVERRIDE;

            static const QString MessageCode;
    };
}

#endif // TYPEISNEVERUSEDTASK_H
