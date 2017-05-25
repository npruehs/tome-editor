#ifndef TYPEFACETVIOLATEDTASK_H
#define TYPEFACETVIOLATEDTASK_H

#include <QObject>

#include "../../Tasks/Controller/task.h"

namespace Tome
{
    class TaskContext;

    /**
     * @brief Finds records with custom type fields whose values are violating one or more restrictions.
     */
    class TypeFacetViolatedTask : public QObject, public Task
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

#endif // TYPEFACETVIOLATEDTASK_H
