#include "recordtreewidgetitem.h"

using namespace Tome;


RecordTreeWidgetItem::RecordTreeWidgetItem(const QString& id, const QString& displayName, const QString& parentId)
    : QTreeWidgetItem((QTreeWidget*)0, QStringList(displayName))
{
    this->setId(id);
    this->setDisplayName(displayName);
    this->setParentId(parentId);
}

QString RecordTreeWidgetItem::getId() const
{
    return this->id;
}

QString RecordTreeWidgetItem::getDisplayName() const
{
    return this->displayName;
}

QString RecordTreeWidgetItem::getParentId() const
{
    return this->parentId;
}

void RecordTreeWidgetItem::setId(const QString& id)
{
    this->id = id;
    this->setData(0, Qt::UserRole, id);
}

void RecordTreeWidgetItem::setDisplayName(const QString& displayName)
{
    this->displayName = displayName;
    this->setText(0, displayName);
}

void RecordTreeWidgetItem::setParentId(const QString& parentId)
{
    this->parentId = parentId;
}
