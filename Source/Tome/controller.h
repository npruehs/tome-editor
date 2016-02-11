#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QSharedPointer>

#include "Projects/Model/project.h"


namespace Tome
{
    class ComponentsController;
    class ExportController;
    class FieldDefinitionsController;
    class RecordsController;
    class TypesController;

    class Controller
    {
        public:
            Controller();
            ~Controller();

            ComponentsController& getComponentsController();
            FieldDefinitionsController& getFieldDefinitionsController();
            RecordsController& getRecordsController();
            ExportController& getExportController();
            TypesController& getTypesController();

            void setProject(QSharedPointer<Project> project);

        private:
            ComponentsController* componentsController;
            FieldDefinitionsController* fieldDefinitionsController;
            RecordsController* recordsController;
            ExportController* exportController;
            TypesController* typesController;
    };
}

#endif // CONTROLLER_H
