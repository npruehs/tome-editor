#include "taskscontroller.h"

#include "task.h"
#include "../Model/taskcontext.h"

#include "../../Components/Controller/componentscontroller.h"
#include "../../Fields/Controller/fielddefinitionscontroller.h"
#include "../../Records/Controller/recordscontroller.h"
#include "../../Types/Controller/typescontroller.h"

using namespace Tome;


TasksController::TasksController(const ComponentsController& componentsController, const FieldDefinitionsController& fieldDefinitionsController, const RecordsController& recordsController, const TypesController& typesController)
    : componentsController(componentsController),
      fieldDefinitionsController(fieldDefinitionsController),
      recordsController(recordsController),
      typesController(typesController)
{
}

TasksController::~TasksController()
{
    for (int i = 0; i < this->tasks.count(); ++i)
    {
        delete this->tasks[i];
    }
}

void TasksController::addTask(Task* task)
{
    this->tasks.append(task);
}

const MessageList TasksController::runAllTasks() const
{
    // Build context.
    TaskContext context(this->componentsController, this->fieldDefinitionsController, this->recordsController, this->typesController);

    // Build message list.
    MessageList messages;

    // Run all tasks.
    for (int i = 0; i < this->tasks.count(); ++i)
    {
        const Task* task = this->tasks.at(i);

        // Run task.
        const MessageList taskMessages = task->execute(context);

        // Compose messages.
        for (int j = 0; j < taskMessages.count(); ++j)
        {
            Message taskMessage = taskMessages.at(j);

            Message message;
            message.content = "[" + task->getId() + "] " + taskMessage.content;
            message.helpLink = taskMessage.helpLink;
            message.severity = taskMessage.severity;
            message.targetSiteId = taskMessage.targetSiteId;
            message.targetSiteType = taskMessage.targetSiteType;

            messages.append(message);
        }
    }

    return messages;
}
