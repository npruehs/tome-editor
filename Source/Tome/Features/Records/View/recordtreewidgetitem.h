#ifndef RECORDTREEWIDGETITEM_H
#define RECORDTREEWIDGETITEM_H

#include <QTreeWidgetItem>

namespace Tome
{
    /**
     * @brief Selectable item in the record hierarchy.
     */
    class RecordTreeWidgetItem : public QTreeWidgetItem
    {
        public:
            /**
             * @brief Constructs a new selectable item in the record hierarchy.
             * @param id Unique id of the record.
             * @param displayName Display name of the record.
             * @param parentId Id of the parent of the record, or null if the record is a root of the hierarchy.
             * @param readOnly Whether to prevent the record from being edited, reparented or removed.
             */
            RecordTreeWidgetItem(const QVariant& id, const QString& displayName, const QVariant& parentId, const bool readOnly);

            /**
             * @brief Gets the unique id of the record.
             * @return Unique id of the record.
             */
            QVariant getId() const;

            /**
             * @brief Gets the display name of the record.
             * @return Display name of the record.
             */
            QString getDisplayName() const;

            /**
             * @brief Gets the id of the parent of the record in the hierarchy.
             * @return Id of the parent of the record, or null if the record is a root of the hierarchy.
             */
            QVariant getParentId() const;

            /**
             * @brief Gets whether to prevent the record from being edited, reparented or removed.
             * @return Whether to prevent the record from being edited, reparented or removed.
             */
            bool isReadOnly() const;

            /**
             * @brief Sets the unique id of the record.
             * @param id New unique id of the record.
             */
            void setId(const QVariant& id);

            /**
             * @brief Sets the display name of the record to show in the hierarchy.
             * @param displayName New display name of the record to show in the hierarchy.
             */
            void setDisplayName(const QString& displayName);

            /**
             * @brief Sets the id of the parent of the record in the hierarchy.
             * @param parentId New id of the parent of the record, or null to make the record a root of the hierarchy.
             */
            void setParentId(const QVariant& parentId);

            /**
             * @brief Sets whether to prevent the record from being edited, reparented or removed.
             * @param readOnly Whether to prevent the record from being edited, reparented or removed.
             */
            void setReadOnly(const bool readOnly);

            bool operator<(const QTreeWidgetItem& other) const Q_DECL_OVERRIDE
            {
                return this->text(0) >= other.text(0);
            }

        private:
            QVariant id;
            QString displayName;
            QVariant parentId;
            bool readOnly;
    };
}

#endif // RECORDTREEWIDGETITEM_H
