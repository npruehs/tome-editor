#ifndef RECORDTREEWIDGET_H
#define RECORDTREEWIDGET_H

#include <QTreeWidget>


namespace Tome
{
    class RecordTreeWidget : public QTreeWidget
    {
        public:
            RecordTreeWidget();

        protected:
            bool dropMimeData(QTreeWidgetItem * parent, int index, const QMimeData * data, Qt::DropAction action);
    };
}


#endif // RECORDTREEWIDGET_H
