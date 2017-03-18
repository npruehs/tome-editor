#ifndef LISTITEMTYPEDOESNOTEXISTTASK_H
#define LISTITEMTYPEDOESNOTEXISTTASK_H

#include <QObject>

#include "../../Tasks/Controller/task.h"

namespace Tome
{
    class TaskContext;

    class ListItemTypeDoesNotExistTask : public QObject, public Task
    {
        public:
            ListItemTypeDoesNotExistTask();

            const QString getDisplayName() const Q_DECL_OVERRIDE;
            const MessageList execute(const TaskContext& context) const Q_DECL_OVERRIDE;

            static const QString MessageCode;
    };
}

#endif // LISTITEMTYPEDOESNOTEXISTTASK_H
