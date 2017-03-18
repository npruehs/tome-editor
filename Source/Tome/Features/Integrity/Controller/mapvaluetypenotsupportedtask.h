#ifndef MAPVALUETYPENOTSUPPORTEDTASK_H
#define MAPVALUETYPENOTSUPPORTEDTASK_H

#include <QObject>

#include "../../Tasks/Controller/task.h"

namespace Tome
{
    class TaskContext;

    class MapValueTypeNotSupportedTask : public QObject, public Task
    {
        public:
            MapValueTypeNotSupportedTask();

            const QString getDisplayName() const Q_DECL_OVERRIDE;
            const MessageList execute(const TaskContext& context) const Q_DECL_OVERRIDE;

            static const QString MessageCode;
    };
}

#endif // MAPVALUETYPENOTSUPPORTEDTASK_H
