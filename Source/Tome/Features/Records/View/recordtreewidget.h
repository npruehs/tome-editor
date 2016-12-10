#ifndef RECORDTREEWIDGET_H
#define RECORDTREEWIDGET_H

#include <QTreeWidget>
#include <QMimeData>

#include "../Model/recordlist.h"

namespace Tome
{
    class RecordsController;
    class RecordTreeWidgetItem;
    class SettingsController;

    class RecordTreeWidget : public QTreeWidget
    {
            Q_OBJECT

        public:
            RecordTreeWidget(RecordsController& recordsController, SettingsController& settingsController);

            void addRecord(const QString& id, const QString& displayName);

            QString getSelectedRecordId() const;
            RecordTreeWidgetItem* getSelectedRecordItem() const;

            void updateRecordItem();
            void updateRecordItem(RecordTreeWidgetItem* recordTreeItem);

            void selectRecord(const QString& id);
            void setRecords(const RecordList& records);

            void removeRecord(const QString& id);

        signals:
            void progressChanged(const QString title, const QString text, const int currentValue, const int maximumValue) const;
            void recordReparented(const QString& recordId, const QString& newParentId);

        protected:
            bool dropMimeData(QTreeWidgetItem * parent, int index, const QMimeData * data, Qt::DropAction action);

        private:
            RecordsController& recordsController;
            SettingsController& settingsController;

            RecordTreeWidgetItem* getRecordItem(const QString& id);
    };
}


#endif // RECORDTREEWIDGET_H
