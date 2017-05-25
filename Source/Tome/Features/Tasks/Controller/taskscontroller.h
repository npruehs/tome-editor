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
            /**
             * @brief Constructs a new controller for runnings tasks on all records.
             * @param componentsController Controller for adding and removing components.
             * @param facetsController Controller for validating type facets.
             * @param fieldDefinitionsController Controller for adding, updating and removing field definitions.
             * @param projectController Controller for creating, loading and saving projects.
             * @param recordsController Controller for adding, updating and removing records.
             * @param typesController Controller for adding, updating and removing custom types.
             */
            TasksController(const ComponentsController& componentsController,
                            const FacetsController& facetsController,
                            const FieldDefinitionsController& fieldDefinitionsController,
                            const ProjectController& projectController,
                            const RecordsController& recordsController,
                            const TypesController& typesController);
            ~TasksController();

            /**
             * @brief Adds the specified task to the list of automated tasks that can be performed by this controller.
             * @param task Task to add.
             */
            void addTask(Task* task);

            /**
             * @brief Runs all registered automated tasks.
             * @return List of messages, warnings and errors generated by all tasks.
             */
            const MessageList runAllTasks() const;

        signals:
            /**
             * @brief Progress of the current tasks operation has changed.
             * @param title Title of the current task.
             * @param text Details of the current task.
             * @param currentValue Current progress value of the tasks operation.
             * @param maximumValue Maximum progress value of the tasks operation.
             */
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
