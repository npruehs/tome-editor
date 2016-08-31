#ifndef TASKSCONTROLLER_H
#define TASKSCONTROLLER_H

#include <QList>

#include "../Model/messagelist.h"


namespace Tome
{
    class Task;

    class ComponentsController;
    class FieldDefinitionsController;
    class RecordsController;
    class TypesController;

    class TasksController
    {
        public:
            TasksController(const ComponentsController& componentsController, const FieldDefinitionsController& fieldDefinitionsController, const RecordsController& recordsController, const TypesController& typesController);
            ~TasksController();

            void addTask(Task* task);
            const MessageList runAllTasks() const;

        private:
            QList<Task*> tasks;

            const ComponentsController& componentsController;
            const FieldDefinitionsController& fieldDefinitionsController;
            const RecordsController& recordsController;
            const TypesController& typesController;
    };
}

#endif // TASKSCONTROLLER_H
