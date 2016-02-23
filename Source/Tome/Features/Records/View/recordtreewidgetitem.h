#ifndef RECORDTREEWIDGETITEM_H
#define RECORDTREEWIDGETITEM_H

#include <QTreeWidgetItem>

namespace Tome
{
    class RecordTreeWidgetItem : public QTreeWidgetItem
    {
        public:
            RecordTreeWidgetItem(const QString& id, const QString& displayName, const QString& parentId);

            QString getId() const;
            QString getDisplayName() const;
            QString getParentId() const;

            void setId(const QString& id);
            void setDisplayName(const QString& displayName);
            void setParentId(const QString& parentId);

        private:
            QString id;
            QString displayName;
            QString parentId;
    };
}

#endif // RECORDTREEWIDGETITEM_H
