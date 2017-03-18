#ifndef TYPEFACETVIOLATEDTASK_H
#define TYPEFACETVIOLATEDTASK_H

#include <QObject>

#include "../../Tasks/Controller/task.h"

namespace Tome
{
    class TaskContext;

    class TypeFacetViolatedTask : public QObject, public Task
    {
        public:
            TypeFacetViolatedTask();

            const QString getDisplayName() const Q_DECL_OVERRIDE;
            const MessageList execute(const TaskContext& context) const Q_DECL_OVERRIDE;

            static const QString MessageCode;
    };
}

#endif // TYPEFACETVIOLATEDTASK_H
