#include "recordtreewidgetitem.h"

using namespace Tome;


RecordTreeWidgetItem::RecordTreeWidgetItem(const QVariant& id, const QString& displayName, const QVariant& parentId, const bool readOnly)
    : QTreeWidgetItem((QTreeWidget*)0, QStringList(displayName))
{
    this->setId(id);
    this->setDisplayName(displayName);
    this->setParentId(parentId);
    this->setReadOnly(readOnly);
}

QVariant RecordTreeWidgetItem::getId() const
{
    return this->id;
}

QString RecordTreeWidgetItem::getDisplayName() const
{
    return this->displayName;
}

QVariant RecordTreeWidgetItem::getParentId() const
{
    return this->parentId;
}

bool RecordTreeWidgetItem::isReadOnly() const
{
    return this->readOnly;
}

void RecordTreeWidgetItem::setId(const QVariant& id)
{
    this->id = id;
    this->setData(0, Qt::UserRole, id);
}

void RecordTreeWidgetItem::setDisplayName(const QString& displayName)
{
    this->displayName = displayName;
    this->setText(0, displayName);
}

void RecordTreeWidgetItem::setParentId(const QVariant& parentId)
{
    this->parentId = parentId;
}

void RecordTreeWidgetItem::setReadOnly(const bool readOnly)
{
    this->readOnly = readOnly;
}
