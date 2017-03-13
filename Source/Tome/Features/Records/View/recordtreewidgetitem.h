#ifndef RECORDTREEWIDGETITEM_H
#define RECORDTREEWIDGETITEM_H

#include <QTreeWidgetItem>

namespace Tome
{
    class RecordTreeWidgetItem : public QTreeWidgetItem
    {
        public:
            RecordTreeWidgetItem(const QVariant& id, const QString& displayName, const QVariant& parentId, const bool readOnly);

            QVariant getId() const;
            QString getDisplayName() const;
            QVariant getParentId() const;
            bool isReadOnly() const;

            void setId(const QVariant& id);
            void setDisplayName(const QString& displayName);
            void setParentId(const QVariant& parentId);
            void setReadOnly(const bool readOnly);

        private:
            QVariant id;
            QString displayName;
            QVariant parentId;
            bool readOnly;
    };
}

#endif // RECORDTREEWIDGETITEM_H
