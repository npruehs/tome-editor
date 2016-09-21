#include "recordtreewidget.h"

#include <QMimeData>

#include "recordtreewidgetitem.h"
#include "../Controller/recordscontroller.h"

using namespace Tome;


RecordTreeWidget::RecordTreeWidget(RecordsController& recordsController)
    : recordsController(recordsController)
{
    this->setDragEnabled(true);
    this->viewport()->setAcceptDrops(true);
    this->setDropIndicatorShown(true);
    this->setHeaderHidden(true);
}

void RecordTreeWidget::addRecord(const QString& id, const QString& displayName)
{
    QTreeWidgetItem* newItem = new RecordTreeWidgetItem(id, displayName, QString(), false);

    this->insertTopLevelItem(0, newItem);
    this->sortItems(0, Qt::AscendingOrder);

    // Select new record.
    this->setCurrentItem(newItem);
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

void RecordTreeWidget::updateRecordItem()
{
    this->updateRecordItem(this->getSelectedRecordItem());
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
    QTreeWidgetItemIterator it(this);

    while (*it)
    {
      if ((*it)->text(0) == id)
      {
          this->setCurrentItem((*it));
          break;
      }

      ++it;
    }
}

void RecordTreeWidget::setRecords(const RecordList& records)
{
    // Create record tree items.
    QMap<QString, RecordTreeWidgetItem*> recordItems;

    for (int i = 0; i < records.size(); ++i)
    {
        const Record& record = records[i];
        RecordTreeWidgetItem* recordItem =
                new RecordTreeWidgetItem(record.id, record.displayName, record.parentId, record.readOnly);
        recordItems.insert(record.id, recordItem);
        updateRecordItem( recordItem );
    }

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
    this->expandAll();
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
