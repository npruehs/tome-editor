#ifndef RECORDTREEWIDGET_H
#define RECORDTREEWIDGET_H

#include <QContextMenuEvent>
#include <QMimeData>
#include <QStack>
#include <QTreeWidget>

#include "../Model/recordlist.h"

namespace Tome
{
    class FacetsController;
    class FieldDefinitionsController;
    class ProjectController;
    class RecordsController;
    class RecordTreeWidgetItem;
    class SettingsController;

    class RecordTreeWidget : public QTreeWidget
    {
            Q_OBJECT

        public:
            RecordTreeWidget(RecordsController& recordsController,
                             FacetsController& facetsController,
                             FieldDefinitionsController& fieldDefinitionsController,
                             ProjectController& projectController,
                             SettingsController& settingsController);

            void addRecord(const QVariant& id, const QString& displayName, const QVariant& parentId);

            QVariant getSelectedRecordId() const;
            QVariantList getSelectedRecordIds() const;

            RecordTreeWidgetItem* getSelectedRecordItem() const;
            QList<RecordTreeWidgetItem*> getSelectedRecordItems() const;

            void navigateForward();
            void navigateBackward();

            void updateRecord(const QVariant& oldId,
                              const QString& oldDisplayName,
                              const QString& oldEditorIconFieldId,
                              const QVariant& newId,
                              const QString& newDisplayName,
                              const QString& newEditorIconFieldId);

            void selectRecord(const QVariant& id, const bool addToHistory);
            void setContextMenuActions(QList<QAction*> actions);
            void setRecords(const RecordList& records);

            void removeRecord(const QVariant& id);

        signals:
            void progressChanged(const QString title, const QString text, const int currentValue, const int maximumValue) const;
            void recordReparented(const QVariant& recordId, const QVariant& newParentId);

        protected:
            void contextMenuEvent(QContextMenuEvent *event) Q_DECL_OVERRIDE;
            bool dropMimeData(QTreeWidgetItem * parent, int index, const QMimeData * data, Qt::DropAction action);
            void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

        private slots:
            void onCurrentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

        private:
            FacetsController& facetsController;
            FieldDefinitionsController& fieldDefinitionsController;
            ProjectController& projectController;
            RecordsController& recordsController;
            SettingsController& settingsController;

            QStack<QVariant> selectedRecordUndoStack;
            QStack<QVariant> selectedRecordRedoStack;
            bool navigating;

            QList<QAction*> contextMenuActions;

            RecordTreeWidgetItem* getRecordItem(const QVariant& id);
            void updateRecordItem(RecordTreeWidgetItem* recordTreeItem);
            void updateRecordItemRecursively(RecordTreeWidgetItem* recordTreeItem);
    };
}


#endif // RECORDTREEWIDGET_H
