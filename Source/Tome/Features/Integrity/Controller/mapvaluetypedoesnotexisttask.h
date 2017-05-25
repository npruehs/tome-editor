#ifndef MAPVALUETYPEDOESNOTEXISTTASK_H
#define MAPVALUETYPEDOESNOTEXISTTASK_H

#include <QObject>

#include "../../Tasks/Controller/task.h"

namespace Tome
{
    class TaskContext;

    /**
     * @brief Finds custom map types whose value type could not be found.
     */
    class MapValueTypeDoesNotExistTask : public QObject, public Task
    {
        public:
            const QString getDisplayName() const Q_DECL_OVERRIDE;
            const MessageList execute(const TaskContext& context) const Q_DECL_OVERRIDE;

            /**
             * @brief Unique message code of this task. Used for looking up more verbose documentation.
             */
            static const QString MessageCode;
    };
}

#endif // MAPVALUETYPEDOESNOTEXISTTASK_H
