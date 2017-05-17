#ifndef LISTWIDGET_H
#define LISTWIDGET_H

#include <QHBoxLayout>
#include <QListWidget>
#include <QString>
#include <QToolButton>
#include <QVBoxLayout>
#include <QWidget>

class ListItemWindow;

namespace Tome
{
    class FacetsController;
    class ProjectController;
    class RecordsController;
    class TypesController;

    /**
     * @brief Allows adding, editing, re-ordering and removing list items.
     */
    class ListWidget : public QWidget
    {
            Q_OBJECT

        public:
            /**
             * @brief Constructs a new widget for adding, editing, re-ordering and removing list items.
             * @param facetsController Controller for validating type facets.
             * @param projectController Controller for creating, loading and saving projects.
             * @param recordsController Controller for adding, updating and removing records.
             * @param typesController Controller for adding, updating and removing custom types.
             * @param parent Optional owner widget.
             */
            explicit ListWidget(FacetsController& facetsController,
                                ProjectController& projectController,
                                RecordsController& recordsController,
                                TypesController& typesController,
                                QWidget *parent = 0);
            ~ListWidget();

            /**
             * @brief Gets the type of the list items.
             * @return Type of the list items.
             */
            QString getItemType() const;

            /**
             * @brief Gets all current list items.
             * @return All current list items.
             */
            QVariantList getItems() const;

            /**
             * @brief Sets the type of the list items.
             * @param itemType Type of the list items.
             */
            void setItemType(const QString& itemType);

            /**
             * @brief Sets all current list items.
             * @param items All current list items.
             */
            void setItems(const QVariantList& items);

        private slots:
            void addItem();
            void editItem(QListWidgetItem* item);
            void removeItem();
            void moveItemUp();
            void moveItemDown();

        private:
            QString itemType;
            QVariantList items;

            ListItemWindow* listItemWindow;

            QHBoxLayout* layout;
            QVBoxLayout* buttonLayout;
            QListWidget* listWidget;

            FacetsController& facetsController;
            ProjectController& projectController;
            RecordsController& recordsController;
            TypesController& typesController;

            void addListWidgetItem(const QVariant& itemData);
            int getSelectedItemIndex() const;
            void updateListWidgetItem(QListWidgetItem* listWidgetItem, const QVariant& itemData);
    };
}
#endif // LISTWIDGET_H
