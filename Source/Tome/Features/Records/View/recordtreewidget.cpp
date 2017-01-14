#include "recordtreewidget.h"

#include <QMenu>
#include <QMimeData>

#include "recordtreewidgetitem.h"
#include "../Controller/recordscontroller.h"
#include "../../Settings/Controller/settingscontroller.h"

using namespace Tome;


RecordTreeWidget::RecordTreeWidget(RecordsController& recordsController, SettingsController& settingsController)
    : recordsController(recordsController),
      settingsController(settingsController),
      navigating(false)
{
    this->setDragEnabled(true);
    this->viewport()->setAcceptDrops(true);
    this->setDropIndicatorShown(true);
    this->setHeaderHidden(true);

    connect(this,
            SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
            SLOT(onCurrentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)));
}

void RecordTreeWidget::addRecord(const QString& id, const QString& displayName, const QString& parentId)
{
    RecordTreeWidgetItem* recordItem = new RecordTreeWidgetItem(id, displayName, QString(), false);
    RecordTreeWidgetItem* parentItem = this->getRecordItem(parentId);

    if (parentItem != nullptr)
    {
        parentItem->addChild(recordItem);
    }
    else
    {
        this->insertTopLevelItem(0, recordItem);
    }

    this->sortItems(0, Qt::AscendingOrder);

    // Select new record.
    this->setCurrentItem(recordItem);
    this->updateRecordItem(recordItem);
}

QString RecordTreeWidget::getSelectedRecordId() const
{
    RecordTreeWidgetItem* recordTreeItem = this->getSelectedRecordItem();

    if (recordTreeItem == 0)
    {
        return QString();
    }

    return recordTreeItem->getId();
}

RecordTreeWidgetItem* RecordTreeWidget::getSelectedRecordItem() const
{
    QList<QTreeWidgetItem*> selectedItems = this->selectedItems();

    if (selectedItems.empty())
    {
        return 0;
    }

    return static_cast<RecordTreeWidgetItem*>(selectedItems.first());
}

void RecordTreeWidget::navigateForward()
{
    if (this->selectedRecordRedoStack.empty())
    {
        return;
    }

    const QString id = this->selectedRecordRedoStack.pop();
    this->selectedRecordUndoStack.push(id);

    // Prevent redo from affecting navigation stacks.
    this->navigating = true;
    this->selectRecord(id);
}

void RecordTreeWidget::navigateBackward()
{
    if (this->selectedRecordUndoStack.count() < 2)
    {
        return;
    }

    const QString id = this->selectedRecordUndoStack.pop();
    this->selectedRecordRedoStack.push(id);

    // Prevent undo from affecting navigation stacks.
    this->navigating = true;
    this->selectRecord(this->selectedRecordUndoStack.top());
}

void RecordTreeWidget::updateRecord(const QString& oldId, const QString& newId, const QString& newDisplayName)
{
    Q_UNUSED(oldId)

    // Update view.
    RecordTreeWidgetItem* recordItem = this->getRecordItem(newId);

    const QString oldDisplayName = recordItem->getDisplayName();
    bool needsSorting = oldDisplayName != newDisplayName;

    recordItem->setDisplayName(newDisplayName);

    // Sort by display name.
    if (needsSorting)
    {
        this->sortItems(0, Qt::AscendingOrder);
    }

    this->updateRecordItem(recordItem);
}

void RecordTreeWidget::updateRecordItem(RecordTreeWidgetItem *recordTreeItem)
{
    if ( nullptr != recordTreeItem )
    {
        const Record &recordData = this->recordsController.getRecord( recordTreeItem->getId() );
        // If the record and all ancestors have no fields, use a folder style icon
        // else use a file style icon
        bool recordIsEmtpy = recordData.fieldValues.empty();
        if ( recordIsEmtpy )
        {
            const RecordList ancestors = this->recordsController.getAncestors( recordTreeItem->getId() );
            for ( int i = 0; ancestors.size() > i && recordIsEmtpy; ++i )
            {
                recordIsEmtpy &= ancestors[ i ].fieldValues.empty();
            }
        }

        // Set color.
        if (recordTreeItem->isReadOnly())
        {
            recordTreeItem->setForeground(0, QBrush(Qt::blue));
        }
        else
        {
            recordTreeItem->setForeground(0, QBrush(Qt::black));
        }

        // Set icon.
        if (recordIsEmtpy)
        {
            recordTreeItem->setIcon(0, QIcon(":/Media/Icons/Folder_6221.png"));
        }
        else
        {
            recordTreeItem->setIcon(0, QIcon(":/Media/Icons/Textfile_818_16x.png"));
        }
    }
}

void RecordTreeWidget::selectRecord(const QString& id)
{
    RecordTreeWidgetItem* item = this->getRecordItem(id);
    this->setCurrentItem(item);
}

void RecordTreeWidget::setContextMenuActions(QList<QAction*> actions)
{
    this->contextMenuActions = actions;
}

void RecordTreeWidget::setRecords(const RecordList& records)
{
    this->clear();

    this->selectedRecordUndoStack.clear();
    this->selectedRecordRedoStack.clear();

    // Create record tree items.
    QMap<QString, RecordTreeWidgetItem*> recordItems;

    for (int i = 0; i < records.size(); ++i)
    {
        const Record& record = records[i];
        RecordTreeWidgetItem* recordItem =
                new RecordTreeWidgetItem(record.id, record.displayName, record.parentId, record.readOnly);
        recordItems.insert(record.id, recordItem);
        updateRecordItem( recordItem );

        // Report progress.
        emit this->progressChanged(tr("Refreshing Records"), record.id, i, records.size());
    }

    // Report finish.
    emit this->progressChanged(tr("Refreshing Records"), QString(), 1, 1);

    // Build hierarchy and prepare item list for tree widget.
    QList<QTreeWidgetItem* > items;

    for (QMap<QString, RecordTreeWidgetItem*>::iterator it = recordItems.begin();
         it != recordItems.end();
         ++it)
    {
        RecordTreeWidgetItem* recordItem = it.value();
        QString recordItemParentId = recordItem->getParentId();
        if (!recordItemParentId.isEmpty())
        {
            if (recordItems.contains(recordItemParentId))
            {
                // Insert into tree.
                RecordTreeWidgetItem* recordParent = recordItems[recordItemParentId];
                recordParent->addChild(recordItem);
            }
            else
            {
                // Reset parent reference.
                this->recordsController.reparentRecord(recordItem->getId(), QString());
            }
        }

        items.append(recordItem);
    }

    // Fill tree widget.
    this->insertTopLevelItems(0, items);
    if (this->settingsController.getExpandRecordTreeOnRefresh())
    {
        this->expandAll();
    }
}

void RecordTreeWidget::removeRecord(const QString& id)
{
    // Update view.
    RecordTreeWidgetItem* recordItem = this->getRecordItem(id);

    if (recordItem->parent() != 0)
    {
        recordItem->parent()->removeChild(recordItem);
    }
    else
    {
        int index = this->indexOfTopLevelItem(recordItem);
        this->takeTopLevelItem(index);
    }

    delete recordItem;
}

void RecordTreeWidget::contextMenuEvent(QContextMenuEvent* event)
{
    QMenu menu(this);

    for (int i = 0; i < this->contextMenuActions.count(); ++i)
    {
        menu.addAction(this->contextMenuActions[i]);
    }

    menu.exec(event->globalPos());
}

bool RecordTreeWidget::dropMimeData(QTreeWidgetItem* parent, int index, const QMimeData* data, Qt::DropAction action)
{
    Q_UNUSED(index)
    Q_UNUSED(action)

    // Get dropped content.
    QByteArray encoded = data->data("application/x-qabstractitemmodeldatalist");
    QDataStream stream(&encoded, QIODevice::ReadOnly);

    // Check if not empty.
    if (!stream.atEnd())
    {
        // Get data.
        int row;
        int col;
        QMap<int,  QVariant> roleDataMap;
        stream >> row >> col >> roleDataMap;

        // Get dragged record.
        QString draggedRecordId = roleDataMap[Qt::UserRole].toString();

        // Get drop target record.
        QString dropTargetRecordId;

        if (parent != 0)
        {
            RecordTreeWidgetItem* dropTarget = static_cast<RecordTreeWidgetItem*>(parent);
            dropTargetRecordId = dropTarget->getId();
        }

        // Emit signal.
        emit recordReparented(draggedRecordId, dropTargetRecordId);
    }

    return true;
}

void RecordTreeWidget::mousePressEvent(QMouseEvent* event)
{
    QModelIndex item = indexAt(event->pos());

    if (item.isValid())
    {
        QTreeView::mousePressEvent(event);
    }
    else
    {
        this->setCurrentItem(nullptr);
    }
}

void RecordTreeWidget::onCurrentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous)
{
    Q_UNUSED(previous)

    if (this->navigating)
    {
        this->navigating = false;
        return;
    }

    // Push id of selected record, or an empty string for "deselected".
    QString selectedRecordId;

    if (current != nullptr)
    {
        RecordTreeWidgetItem* item = static_cast<RecordTreeWidgetItem*>(current);
        selectedRecordId = item->getId();
    }

    this->selectedRecordUndoStack.push(selectedRecordId);
    this->selectedRecordRedoStack.clear();
}

RecordTreeWidgetItem* RecordTreeWidget::getRecordItem(const QString& id)
{
    QTreeWidgetItemIterator it(this);

    while (*it)
    {
        RecordTreeWidgetItem *item = static_cast<RecordTreeWidgetItem*>(*it);
        if (item->getId() == id)
        {
            return item;
        }
        ++it;
    }

    return nullptr;
}
