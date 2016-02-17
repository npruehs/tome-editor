#include "recordtreewidgetitem.h"

using namespace Tome;


RecordTreeWidgetItem::RecordTreeWidgetItem(const QString& id, const QString& displayName)
    : QTreeWidgetItem((QTreeWidget*)0, QStringList(displayName)),
      id(id),
      displayName(displayName)
{
}

QString RecordTreeWidgetItem::getId() const
{
    return this->id;
}

QString RecordTreeWidgetItem::getDisplayName() const
{
    return this->displayName;
}

void RecordTreeWidgetItem::setId(const QString& id)
{
    this->id = id;
}

void RecordTreeWidgetItem::setDisplayName(const QString& displayName)
{
    this->displayName = displayName;
    this->setText(0, displayName);
}
