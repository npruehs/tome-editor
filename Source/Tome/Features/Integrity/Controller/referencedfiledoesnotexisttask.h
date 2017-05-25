#ifndef REFERENCEDFILEDOESNOTEXISTTASK_H
#define REFERENCEDFILEDOESNOTEXISTTASK_H

#include <QObject>

#include "../../Tasks/Controller/task.h"

namespace Tome
{
    class TaskContext;

    /**
     * @brief Finds records with File type fields who are referencing files that could not be found.
     */
    class ReferencedFileDoesNotExistTask : public QObject, public Task
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

#endif // REFERENCEDFILEDOESNOTEXISTTASK_H
