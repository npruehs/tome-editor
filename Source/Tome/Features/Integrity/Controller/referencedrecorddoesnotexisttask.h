#ifndef REFERENCEDRECORDDOESNOTEXISTTASK_H
#define REFERENCEDRECORDDOESNOTEXISTTASK_H

#include <QObject>

#include "../../Tasks/Controller/task.h"

namespace Tome
{
    class TaskContext;

    /**
     * @brief Finds records with Reference type fields who are referencing other records that could not be found.
     */
    class ReferencedRecordDoesNotExistTask : public QObject, public Task
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

#endif // REFERENCEDRECORDDOESNOTEXISTTASK_H
