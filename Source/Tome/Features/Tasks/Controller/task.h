#ifndef TASK
#define TASK

#include "../Model/messagelist.h"


namespace Tome
{
    class TaskContext;

    class Task
    {
        public:
            Task();
            virtual ~Task();

            virtual const QString getDisplayName() const = 0;
            virtual const MessageList execute(const TaskContext& context) const = 0;
    };
}
#endif // TASK

