#include "recordtreewidget.h"

#include <QMimeData>

#include <QMessageBox>

using namespace Tome;

RecordTreeWidget::RecordTreeWidget()
{
}

bool RecordTreeWidget::dropMimeData(QTreeWidgetItem* parent, int index, const QMimeData* data, Qt::DropAction action)
{
    QMessageBox::information(this, "Drop Successful", data->text(), QMessageBox::Ok);
}
