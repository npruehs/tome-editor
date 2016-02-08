#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QSharedPointer>

#include "Projects/project.h"


namespace Tome
{
    class ComponentsController;

    class Controller
    {
        public:
            Controller();
            ~Controller();

            ComponentsController& getComponentsController();

            void setProject(QSharedPointer<Project> project);

        private:
            ComponentsController* componentsController;
    };
}

#endif // CONTROLLER_H
