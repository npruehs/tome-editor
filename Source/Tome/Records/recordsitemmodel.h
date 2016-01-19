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
            void updateRecord(const QString displayName, const QString& newId, const QString& newDisplayName);
            void removeRecord(const QString& displayName);

            QStandardItem* findItem(const QString& text);

        private:
            QSharedPointer<Tome::Project> project;

            void insertItem(const QString& text);
            void updateItem(const QString& oldText, const QString& newText);
            void removeItem(const QString& text);
    };
}
#endif // RECORDSITEMMODEL_H
