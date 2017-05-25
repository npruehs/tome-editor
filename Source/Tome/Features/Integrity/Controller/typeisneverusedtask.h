#ifndef TYPEISNEVERUSEDTASK_H
#define TYPEISNEVERUSEDTASK_H

#include <QObject>

#include "../../Tasks/Controller/task.h"

namespace Tome
{
    class TaskContext;

    /**
     * @brief Finds custom types who aren't used by any field definitions.
     */
    class TypeIsNeverUsedTask : public QObject, public Task
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

#endif // TYPEISNEVERUSEDTASK_H
