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

    /**
     * @brief Shows the record of hierarchy and allows selecting one or more records.
     */
    class RecordTreeWidget : public QTreeWidget
    {
            Q_OBJECT

        public:
            /**
             * @brief Constructs a new widget that shows the record of hierarchy and allows selecting one or more records.
             * @param recordsController Controller for adding, updating and removing records.
             * @param facetsController Controller for validating type facets.
             * @param fieldDefinitionsController Controller for adding, updating and removing field definitions.
             * @param projectController Controller for creating, loading and saving projects.
             * @param settingsController Controller for accessing user settings.
             */
            RecordTreeWidget(RecordsController& recordsController,
                             FacetsController& facetsController,
                             FieldDefinitionsController& fieldDefinitionsController,
                             ProjectController& projectController,
                             SettingsController& settingsController);

            /**
             * @brief Adds the specified record to show in the hierarchy.
             * @param id Id of the record to add.
             * @param displayName Display name of the record to show in the hierarchy.
             * @param parentId Id of the record to parent the new record to in the hierarchy.
             */
            void addRecord(const QVariant& id, const QString& displayName, const QVariant& parentId);

            /**
             * @brief Gets the id of the currently selected record.
             * @return Id of the currently selected record, or null if no record is selected.
             */
            QVariant getSelectedRecordId() const;

            /**
             * @brief Gets a list of ids of all currently selected records.
             * @return List of ids of all currently selected records, or an empty list if no record is selected.
             */
            QVariantList getSelectedRecordIds() const;

            /**
             * @brief Selects the next record in the history of selected records.
             */
            void navigateForward();

            /**
             * @brief Selects the previous record in the history of selected records.
             */
            void navigateBackward();

            /**
             * @brief Updates the specified record in the hierarchy.
             * @param oldId Previous id of the record to update.
             * @param oldDisplayName Previous display name of the record to update to show in the hierarchy.
             * @param oldEditorIconFieldId Previous id of the field to use as editor icon for the record to update.
             * @param newId New id of the record to update.
             * @param newDisplayName New display name of the record to update to show in the hierarchy.
             * @param newEditorIconFieldId New id of the field to use as editor icon for the record to update.
             */
            void updateRecord(const QVariant& oldId,
                              const QString& oldDisplayName,
                              const QString& oldEditorIconFieldId,
                              const QVariant& newId,
                              const QString& newDisplayName,
                              const QString& newEditorIconFieldId);

            /**
             * @brief Selects the specified record in the hierarchy.
             * @param id Id of the record to select.
             * @param addToHistory Whether to add the record to the navigation history, or not.
             */
            void selectRecord(const QVariant& id, const bool addToHistory);

            /**
             * @brief Sets the list of actions to show in the context menu of the selected records.
             * @param actions List of actions to show in the context menu of the selected records.
             */
            void setContextMenuActions(QList<QAction*> actions);

            /**
             * @brief Sets the records to show in the hierarchy.
             * @param records Records to show in the hierarchy.
             */
            void setRecords(const RecordList& records);

            /**
             * @brief Removes the specified record from the hierarchy.
             * @param id Id of the record to remove.
             */
            void removeRecord(const QVariant& id);

        signals:
            /**
             * @brief Progress of the current record operation has changed.
             * @param title Title of the current record operation step.
             * @param text Details of the current record operation step.
             * @param currentValue Current progress value of the current record operation step.
             * @param maximumValue Maximum progress value of the current record operation step.
             */
            void progressChanged(const QString title, const QString text, const int currentValue, const int maximumValue) const;

            /**
             * @brief A record has been reparented to another record in the hierarchy.
             * @param recordId Id of the record which has been reparented.
             * @param newParentId Id of the new parent of the record, or null if the record has been made a root of the hierarchy.
             */
            void recordReparented(const QVariant& recordId, const QVariant& newParentId);

        protected:
            /**
             * @brief Shows the context menu for the current set of selected records.
             * @param event Event for showing the context menu.
             */
            void contextMenuEvent(QContextMenuEvent *event) Q_DECL_OVERRIDE;

            /**
             * @brief Checks whether to reparent a record in the hierarchy by drag & drop.
             * @param parent New parent to reparent the dragged record to.
             * @param index Unused.
             * @param data Dragged record.
             * @param action Unused.
             * @return True, if a record has been dragged & dropped onto another one, and false otherwise.
             */
            bool dropMimeData(QTreeWidgetItem* parent, int index, const QMimeData* data, Qt::DropAction action) Q_DECL_OVERRIDE;

            /**
             * @brief Selects a record in the hierarchy, or clears the current selection.
             * @param event Event to selecting a record in the hierarchy or clearing the selection.
             */
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
            RecordTreeWidgetItem* getSelectedRecordItem() const;
            QList<RecordTreeWidgetItem*> getSelectedRecordItems() const;
            void sort();
            void updateRecordItem(RecordTreeWidgetItem* recordTreeItem);
            void updateRecordItemRecursively(RecordTreeWidgetItem* recordTreeItem);
    };
}


#endif // RECORDTREEWIDGET_H
