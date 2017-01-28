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

            const QString getDisplayName() const;
            const MessageList execute(const TaskContext& context) const;

            static const QString MessageCode;
    };
}

#endif // TYPEFACETVIOLATEDTASK_H
