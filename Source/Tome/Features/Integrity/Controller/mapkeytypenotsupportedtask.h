#ifndef MAPKEYTYPENOTSUPPORTEDTASK_H
#define MAPKEYTYPENOTSUPPORTEDTASK_H

#include <QObject>

#include "../../Tasks/Controller/task.h"

namespace Tome
{
    class TaskContext;

    class MapKeyTypeNotSupportedTask : public QObject, public Task
    {
        public:
            MapKeyTypeNotSupportedTask();

            const QString getDisplayName() const Q_DECL_OVERRIDE;
            const MessageList execute(const TaskContext& context) const Q_DECL_OVERRIDE;

            static const QString MessageCode;
    };
}

#endif // MAPKEYTYPENOTSUPPORTEDTASK_H
