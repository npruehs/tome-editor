#ifndef FIELDALWAYSHASITSDEFAULTVALUETASK_H
#define FIELDALWAYSHASITSDEFAULTVALUETASK_H

#include <QObject>

#include "../../Tasks/Controller/task.h"

namespace Tome
{
    class TaskContext;

    /**
     * @brief Finds fields whose value is never changed for any record.
     */
    class FieldAlwaysHasItsDefaultValueTask : public QObject, public Task
    {
        public:
            const QString getDisplayName() const Q_DECL_OVERRIDE;
            const MessageList execute(const TaskContext& context) const Q_DECL_OVERRIDE;

            /**
             * @brief Unique message code of this task. Used for looking up more verbose documentation.
             */
            static const QString MessageCode;
    };
}

#endif // FIELDALWAYSHASITSDEFAULTVALUETASK_H
