#ifndef RECORDTREEWIDGETITEM_H
#define RECORDTREEWIDGETITEM_H

#include <QTreeWidgetItem>

namespace Tome
{
    class RecordTreeWidgetItem : public QTreeWidgetItem
    {
        public:
            RecordTreeWidgetItem(const QString& id, const QString& displayName, const QString& parentId, const bool readOnly);

            QString getId() const;
            QString getDisplayName() const;
            QString getParentId() const;
            bool isReadOnly() const;

            void setId(const QString& id);
            void setDisplayName(const QString& displayName);
            void setParentId(const QString& parentId);
            void setReadOnly(const bool readOnly);

        private:
            QString id;
            QString displayName;
            QString parentId;
            bool readOnly;
    };
}

#endif // RECORDTREEWIDGETITEM_H
