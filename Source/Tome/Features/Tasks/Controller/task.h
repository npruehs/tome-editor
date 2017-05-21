#ifndef TASK
#define TASK

#include "../Model/messagelist.h"


namespace Tome
{
    class TaskContext;

    /**
     * @brief Automated task that may result in a list of messages, warnings and errors.
     */
    class Task
    {
        public:
            /**
             * @brief Constructs a new automated task that may result in a list of messages, warnings and errors.
             */
            Task();
            virtual ~Task();

            /**
             * @brief Gets the localized display name of this task, as shown next to progress bars.
             * @return Localized display name of this task.
             */
            virtual const QString getDisplayName() const = 0;

            /**
             * @brief Executes this task, generating a list of messages, warnings and errors.
             * @param context Controllers required for executing the task.
             * @return List of messages, warnings and errors.
             */
            virtual const MessageList execute(const TaskContext& context) const = 0;
    };
}
#endif // TASK

