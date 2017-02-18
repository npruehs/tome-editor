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

    class TaskContext
    {
        public:
            TaskContext(
                    const ComponentsController& componentsController,
                    const FacetsController& facetsController,
                    const FieldDefinitionsController& fieldDefinitionsController,
                    const ProjectController& projectController,
                    const RecordsController& recordsController,
                    const TypesController& typesController);

            const ComponentsController& componentsController;
            const FacetsController& facetsController;
            const FieldDefinitionsController& fieldDefinitionsController;
            const ProjectController& projectController;
            const RecordsController& recordsController;
            const TypesController& typesController;
    };
}

#endif // TASKCONTEXT_H
