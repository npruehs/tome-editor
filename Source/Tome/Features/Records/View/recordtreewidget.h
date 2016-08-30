#ifndef RECORDTREEWIDGET_H
#define RECORDTREEWIDGET_H

#include <QTreeWidget>
#include <QMimeData>

#include "../Model/recordlist.h"

namespace Tome
{
    class RecordsController;
    class RecordTreeWidgetItem;

    class RecordTreeWidget : public QTreeWidget
    {
            Q_OBJECT

        public:
            RecordTreeWidget(RecordsController& recordsController);

            void addRecord(const QString& id, const QString& displayName);

            QString getSelectedRecordId() const;
            RecordTreeWidgetItem* getSelectedRecordItem() const;
            void updateRecordIcon( RecordTreeWidgetItem *recordTreeItem );

            void setRecords(const RecordList& records);

        signals:
            void recordReparented(const QString& recordId, const QString& newParentId);

        protected:
            bool dropMimeData(QTreeWidgetItem * parent, int index, const QMimeData * data, Qt::DropAction action);

        private:

            RecordsController& recordsController;
    };
}


#endif // RECORDTREEWIDGET_H
