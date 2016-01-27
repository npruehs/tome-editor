#ifndef LISTITEMMODEL_H
#define LISTITEMMODEL_H

#include <QStandardItemModel>


namespace Tome
{
    class ListItemModel : public QStandardItemModel
    {
        public:
            ListItemModel(QStringList& list);

            void addItem(const QString& item);
            void removeItem(const int index);

        private:
            QStringList list;
    };
}

#endif // LISTITEMMODEL_H
