#ifndef LISTWIDGET_H
#define LISTWIDGET_H

#include <QAction>
#include <QHBoxLayout>
#include <QListView>
#include <QString>
#include <QToolButton>
#include <QWidget>

#include "listitemmodel.h"
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

        private:
            QString fieldType;

            ListItemWindow* listItemWindow;

            QHBoxLayout* layout;
            QListView* listView;

            QSharedPointer<ListItemModel> viewModel;
    };
}
#endif // LISTWIDGET_H
