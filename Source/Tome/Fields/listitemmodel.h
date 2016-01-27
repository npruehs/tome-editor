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
            QString removeItem(const int index);
            void moveItemUp(const int index);
            void moveItemDown(const int index);

        private:
            QStringList list;

            void addItem(int index, const QString& item);
    };
}

#endif // LISTITEMMODEL_H
