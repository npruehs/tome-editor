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

            const QString getDisplayName() const Q_DECL_OVERRIDE;
            const MessageList execute(const TaskContext& context) const Q_DECL_OVERRIDE;

            static const QString MessageCode;
    };
}

#endif // REFERENCEDRECORDDOESNOTEXISTTASK_H
