#ifndef CUSTOMFIELDTYPESITEMMODEL_H
#define CUSTOMFIELDTYPESITEMMODEL_H

#include <QStandardItemModel>

#include "../Projects/project.h"

namespace Tome
{
    class CustomTypesItemModel : public QStandardItemModel
    {
        public:
            CustomTypesItemModel(QSharedPointer<Tome::Project> project);

            void addEnumeration(const QString& name, const QStringList& enumeration);
            void updateEnumeration(const int index, const QString& name, const QStringList& enumeration);
            void removeCustomType(const int index);

        private:
            QSharedPointer<Tome::Project> project;

            void insertItem(const QString& text);
            void updateItem(const QString& oldText, const QString& newText);
    };
}

#endif // CUSTOMFIELDTYPESITEMMODEL_H
