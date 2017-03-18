#ifndef COMPONENTHASNOFIELDSTASK_H
#define COMPONENTHASNOFIELDSTASK_H

#include <QObject>

#include "../../Tasks/Controller/task.h"

namespace Tome
{
    class TaskContext;

    class ComponentHasNoFieldsTask : public QObject, public Task
    {
        public:
            ComponentHasNoFieldsTask();

            const QString getDisplayName() const Q_DECL_OVERRIDE;
            const MessageList execute(const TaskContext& context) const Q_DECL_OVERRIDE;

            static const QString MessageCode;
    };
}

#endif // COMPONENTHASNOFIELDSTASK_H
