#ifndef REFERENCEDFILEDOESNOTEXISTTASK_H
#define REFERENCEDFILEDOESNOTEXISTTASK_H

#include <QObject>

#include "../../Tasks/Controller/task.h"

namespace Tome
{
    class TaskContext;

    class ReferencedFileDoesNotExistTask : public QObject, public Task
    {
        public:
            ReferencedFileDoesNotExistTask();

            const QString getDisplayName() const;
            const MessageList execute(const TaskContext& context) const;

            static const QString MessageCode;
    };
}

#endif // REFERENCEDFILEDOESNOTEXISTTASK_H
