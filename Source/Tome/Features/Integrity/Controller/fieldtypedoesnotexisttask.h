#ifndef FIELDTYPEDOESNOTEXISTTASK_H
#define FIELDTYPEDOESNOTEXISTTASK_H

#include <QObject>

#include "../../Tasks/Controller/task.h"

namespace Tome
{
    class TaskContext;

    class FieldTypeDoesNotExistTask : public QObject, public Task
    {
        public:
            FieldTypeDoesNotExistTask();

            const QString getDisplayName() const Q_DECL_OVERRIDE;
            const MessageList execute(const TaskContext& context) const Q_DECL_OVERRIDE;

            static const QString MessageCode;
    };
}

#endif // FIELDTYPEDOESNOTEXISTTASK_H
