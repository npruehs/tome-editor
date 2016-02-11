#ifndef LISTWIDGET_H
#define LISTWIDGET_H

#include <QAction>
#include <QHBoxLayout>
#include <QListWidget>
#include <QString>
#include <QToolButton>
#include <QWidget>

#include "listitemwindow.h"


namespace Tome
{
    /**
     * @brief Allows adding, editing, re-ordering and removing list items.
     */
    class ListWidget : public QWidget
    {
            Q_OBJECT

        public:
            explicit ListWidget(QWidget *parent = 0);
            ~ListWidget();

            QString getFieldType() const;

            void setFieldType(const QString& fieldType);

        private slots:
            void addItem();
            void removeItem();
            void moveItemUp();
            void moveItemDown();

        private:
            QString fieldType;
            QStringList items;

            ListItemWindow* listItemWindow;

            QHBoxLayout* layout;
            QListWidget* listWidget;

            int getSelectedItemIndex() const;
    };
}
#endif // LISTWIDGET_H
