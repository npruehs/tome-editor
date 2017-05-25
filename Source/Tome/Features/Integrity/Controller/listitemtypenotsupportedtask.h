#ifndef LISTITEMTYPENOTSUPPORTEDTASK_H
#define LISTITEMTYPENOTSUPPORTEDTASK_H

#include <QObject>

#include "../../Tasks/Controller/task.h"

namespace Tome
{
    class TaskContext;

    /**
     * @brief Finds custom list types whose item type is not yet supported.
     */
    class ListItemTypeNotSupportedTask : public QObject, public Task
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

#endif // LISTITEMTYPENOTSUPPORTEDTASK_H
