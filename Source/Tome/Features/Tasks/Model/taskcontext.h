#ifndef TASKCONTEXT_H
#define TASKCONTEXT_H

namespace Tome
{
    class ComponentsController;
    class FacetsController;
    class FieldDefinitionsController;
    class ProjectController;
    class RecordsController;
    class TypesController;

    /**
     * @brief Context to run automated tasks in.
     */
    class TaskContext
    {
        public:
            /**
             * @brief Constructs a new context to run automated tasks in.
             * @param componentsController Controller for adding and removing components.
             * @param facetsController Controller for validating type facets.
             * @param fieldDefinitionsController Controller for adding, updating and removing field definitions.
             * @param projectController Controller for creating, loading and saving projects.
             * @param recordsController Controller for adding, updating and removing records.
             * @param typesController Controller for adding, updating and removing custom types.
             */
            TaskContext(
                    const ComponentsController& componentsController,
                    const FacetsController& facetsController,
                    const FieldDefinitionsController& fieldDefinitionsController,
                    const ProjectController& projectController,
                    const RecordsController& recordsController,
                    const TypesController& typesController);

            /**
             * @brief Controller for adding and removing components.
             */
            const ComponentsController& componentsController;

            /**
             * @brief Controller for validating type facets.
             */
            const FacetsController& facetsController;

            /**
             * @brief Controller for adding, updating and removing field definitions.
             */
            const FieldDefinitionsController& fieldDefinitionsController;

            /**
             * @brief Controller for creating, loading and saving projects.
             */
            const ProjectController& projectController;

            /**
             * @brief Controller for adding, updating and removing records.
             */
            const RecordsController& recordsController;

            /**
             * @brief Controller for adding, updating and removing custom types.
             */
            const TypesController& typesController;
    };
}

#endif // TASKCONTEXT_H
