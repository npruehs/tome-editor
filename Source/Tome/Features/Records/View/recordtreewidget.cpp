#include "recordtreewidget.h"

#include <QMimeData>

#include "recordtreewidgetitem.h"

using namespace Tome;


RecordTreeWidget::RecordTreeWidget()
{
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
        RecordTreeWidgetItem* dropTarget = static_cast<RecordTreeWidgetItem*>(parent);
        QString dropTargetRecordId = dropTarget->getId();

        // Emit signal.
        emit recordReparented(draggedRecordId, dropTargetRecordId);
    }

    return true;
}
