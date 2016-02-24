#ifndef RECORDTREEWIDGET_H
#define RECORDTREEWIDGET_H

#include <QTreeWidget>
#include <QMimeData>

namespace Tome
{
    class RecordTreeWidget : public QTreeWidget
    {
            Q_OBJECT

        public:
            RecordTreeWidget();

        signals:
            void recordReparented(const QString& recordId, const QString& newParentId);

        protected:
            bool dropMimeData(QTreeWidgetItem * parent, int index, const QMimeData * data, Qt::DropAction action);
    };
}


#endif // RECORDTREEWIDGET_H
