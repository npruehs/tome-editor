#ifndef REFERENCEDRECORDDOESNOTEXISTTASK_H
#define REFERENCEDRECORDDOESNOTEXISTTASK_H

#include <QObject>

#include "../../Tasks/Controller/task.h"

namespace Tome
{
    class TaskContext;

    class ReferencedRecordDoesNotExistTask : public QObject, public Task
    {
        public:
            ReferencedRecordDoesNotExistTask();

            const QString getDisplayName() const;
            const MessageList execute(const TaskContext& context) const;

            static const QString MessageCode;
    };
}

#endif // REFERENCEDRECORDDOESNOTEXISTTASK_H
