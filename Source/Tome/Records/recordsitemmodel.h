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

            void addRecord(const QString& id);

        private:
            QSharedPointer<Tome::Project> project;
    };
}
#endif // RECORDSITEMMODEL_H
