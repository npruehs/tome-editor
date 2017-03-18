#ifndef FIELDALWAYSHASITSDEFAULTVALUETASK_H
#define FIELDALWAYSHASITSDEFAULTVALUETASK_H

#include <QObject>

#include "../../Tasks/Controller/task.h"

namespace Tome
{
    class TaskContext;

    class FieldAlwaysHasItsDefaultValueTask : public QObject, public Task
    {
        public:
            FieldAlwaysHasItsDefaultValueTask();

            const QString getDisplayName() const Q_DECL_OVERRIDE;
            const MessageList execute(const TaskContext& context) const Q_DECL_OVERRIDE;

            static const QString MessageCode;
    };
}

#endif // FIELDALWAYSHASITSDEFAULTVALUETASK_H
