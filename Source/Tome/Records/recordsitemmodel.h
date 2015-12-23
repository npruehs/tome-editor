#ifndef RECORDSITEMMODEL_H
#define RECORDSITEMMODEL_H

#include <QStandardItemModel>

#include "../Projects/project.h"

namespace Tome
{
    class RecordsItemModel : public QStandardItemModel
    {
        public:
            RecordsItemModel(QSharedPointer<Tome::Project> project);

            void addRecord(const QString& id, const QString& displayName);

        private:
            QSharedPointer<Tome::Project> project;

            void insertItem(const QString& text);
    };
}
#endif // RECORDSITEMMODEL_H
