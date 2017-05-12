#ifndef TASKSCONTROLLER_H
#define TASKSCONTROLLER_H

#include <QList>
#include <QObject>

#include "../Model/messagelist.h"


namespace Tome
{
    class Task;

    class ComponentsController;
    class FacetsController;
    class FieldDefinitionsController;
    class ProjectController;
    class RecordsController;
    class TypesController;

    /**
     * @brief Controller for runnings tasks on all records.
     */
    class TasksController : public QObject
    {
            Q_OBJECT

        public:
            TasksController(const ComponentsController& componentsController,
                            const FacetsController& facetsController,
                            const FieldDefinitionsController& fieldDefinitionsController,
                            const ProjectController& projectController,
                            const RecordsController& recordsController,
                            const TypesController& typesController);
            ~TasksController();

            void addTask(Task* task);
            const MessageList runAllTasks() const;

        signals:
            void progressChanged(const QString title, const QString text, const int currentValue, const int maximumValue) const;

        private:
            QList<Task*> tasks;

            const ComponentsController& componentsController;
            const FacetsController& facetsController;
            const FieldDefinitionsController& fieldDefinitionsController;
            const ProjectController& projectController;
            const RecordsController& recordsController;
            const TypesController& typesController;
    };
}

#endif // TASKSCONTROLLER_H
