#include "taskscontroller.h"

#include "task.h"
#include "../Model/taskcontext.h"

#include "../../Components/Controller/componentscontroller.h"
#include "../../Fields/Controller/fielddefinitionscontroller.h"
#include "../../Records/Controller/recordscontroller.h"
#include "../../Types/Controller/typescontroller.h"

using namespace Tome;


TasksController::TasksController(const ComponentsController& componentsController,
        const FacetsController& facetsController,
        const FieldDefinitionsController& fieldDefinitionsController,
        const ProjectController& projectController,
        const RecordsController& recordsController,
        const TypesController& typesController)
    : componentsController(componentsController),
      facetsController(facetsController),
      fieldDefinitionsController(fieldDefinitionsController),
      projectController(projectController),
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
    TaskContext context(
                this->componentsController,
                this->facetsController,
                this->fieldDefinitionsController,
                this->projectController,
                this->recordsController,
                this->typesController);

    // Build message list.
    MessageList messages;

    // Run all tasks.
    for (int i = 0; i < this->tasks.count(); ++i)
    {
        const Task* task = this->tasks.at(i);

        // Update progress.
        emit this->progressChanged(tr("Running Tasks"), task->getDisplayName(), i, this->tasks.count());

        // Run task.
        const MessageList taskMessages = task->execute(context);

        // Compose messages.
        for (int j = 0; j < taskMessages.count(); ++j)
        {
            Message taskMessage = taskMessages.at(j);
            messages.append(taskMessage);
        }
    }

    emit this->progressChanged(tr("Running Tasks"), QString(), 1, 1);

    return messages;
}
