#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QSharedPointer>

#include "Projects/project.h"


namespace Tome
{
    class ComponentsController;
    class ExportController;
    class FieldDefinitionsController;
    class RecordsController;

    class Controller
    {
        public:
            Controller();
            ~Controller();

            ComponentsController& getComponentsController();
            FieldDefinitionsController& getFieldDefinitionsController();
            RecordsController& getRecordsController();
            ExportController& getExportController();

            void setProject(QSharedPointer<Project> project);

        private:
            ComponentsController* componentsController;
            FieldDefinitionsController* fieldDefinitionsController;
            RecordsController* recordsController;
            ExportController* exportController;
    };
}

#endif // CONTROLLER_H
