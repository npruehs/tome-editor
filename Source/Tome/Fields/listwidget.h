#ifndef LISTWIDGET_H
#define LISTWIDGET_H

#include <QHBoxLayout>
#include <QListView>
#include <QString>
#include <QWidget>


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

        private:
            QString fieldType;

            QHBoxLayout* layout;
            QListView* listView;
    };
}
#endif // LISTWIDGET_H
