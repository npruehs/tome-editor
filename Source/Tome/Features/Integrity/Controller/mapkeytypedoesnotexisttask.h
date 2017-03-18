#ifndef MAPKEYTYPEDOESNOTEXISTTASK_H
#define MAPKEYTYPEDOESNOTEXISTTASK_H

#include <QObject>

#include "../../Tasks/Controller/task.h"

namespace Tome
{
    class TaskContext;

    class MapKeyTypeDoesNotExistTask : public QObject, public Task
    {
        public:
            MapKeyTypeDoesNotExistTask();

            const QString getDisplayName() const Q_DECL_OVERRIDE;
            const MessageList execute(const TaskContext& context) const Q_DECL_OVERRIDE;

            static const QString MessageCode;
    };
}

#endif // MAPKEYTYPEDOESNOTEXISTTASK_H
