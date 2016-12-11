#ifndef RECORDTREEWIDGET_H
#define RECORDTREEWIDGET_H

#include <QTreeWidget>
#include <QMimeData>
#include <QStack>

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

            void addRecord(const QString& id, const QString& displayName, const QString& parentId);

            QString getSelectedRecordId() const;
            RecordTreeWidgetItem* getSelectedRecordItem() const;

            void navigateForward();
            void navigateBackward();

            void updateRecord(const QString& oldId, const QString& newId, const QString& newDisplayName);

            void selectRecord(const QString& id);
            void setRecords(const RecordList& records);

            void removeRecord(const QString& id);

        signals:
            void progressChanged(const QString title, const QString text, const int currentValue, const int maximumValue) const;
            void recordReparented(const QString& recordId, const QString& newParentId);

        protected:
            bool dropMimeData(QTreeWidgetItem * parent, int index, const QMimeData * data, Qt::DropAction action);

        private slots:
            void onCurrentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

        private:
            RecordsController& recordsController;
            SettingsController& settingsController;

            QStack<QString> selectedRecordUndoStack;
            QStack<QString> selectedRecordRedoStack;
            bool navigating;

            RecordTreeWidgetItem* getRecordItem(const QString& id);
            void updateRecordItem(RecordTreeWidgetItem* recordTreeItem);
    };
}


#endif // RECORDTREEWIDGET_H
