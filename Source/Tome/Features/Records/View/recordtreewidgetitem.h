#ifndef RECORDTREEWIDGETITEM_H
#define RECORDTREEWIDGETITEM_H

#include <QTreeWidgetItem>

namespace Tome
{
    class RecordTreeWidgetItem : public QTreeWidgetItem
    {
        public:
            RecordTreeWidgetItem(const QString& id, const QString& displayName);

            QString getId() const;
            QString getDisplayName() const;

            void setId(const QString& id);
            void setDisplayName(const QString& displayName);

        private:
            QString id;
            QString displayName;
    };
}

#endif // RECORDTREEWIDGETITEM_H
