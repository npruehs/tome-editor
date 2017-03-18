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
            explicit ListWidget(FacetsController& facetsController,
                                ProjectController& projectController,
                                RecordsController& recordsController,
                                TypesController& typesController,
                                QWidget *parent = 0);
            ~ListWidget();

            QString getItemType() const;
            QVariantList getItems() const;

            void setItemType(const QString& itemType);
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
