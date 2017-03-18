#ifndef MAPVALUETYPEDOESNOTEXISTTASK_H
#define MAPVALUETYPEDOESNOTEXISTTASK_H

#include <QObject>

#include "../../Tasks/Controller/task.h"

namespace Tome
{
    class TaskContext;

    class MapValueTypeDoesNotExistTask : public QObject, public Task
    {
        public:
            MapValueTypeDoesNotExistTask();

            const QString getDisplayName() const Q_DECL_OVERRIDE;
            const MessageList execute(const TaskContext& context) const Q_DECL_OVERRIDE;

            static const QString MessageCode;
    };
}

#endif // MAPVALUETYPEDOESNOTEXISTTASK_H
