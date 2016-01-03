#ifndef COMPONENTSITEMMODEL_H
#define COMPONENTSITEMMODEL_H

#include <QStandardItemModel>

#include "../Projects/project.h"

namespace Tome
{
    class ComponentsItemModel : public QStandardItemModel
    {
        public:
            ComponentsItemModel(QSharedPointer<Tome::Project> project);

            void addComponent(const QString& componentName);

            void removeComponent(const int index);
            void removeComponent(const QString& componentName);

        private:
            QSharedPointer<Tome::Project> project;

            void insertItem(const QString& text);
            void removeItem(const QString& text);
    };
}

#endif // COMPONENTSITEMMODEL_H
