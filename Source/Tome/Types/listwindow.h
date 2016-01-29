#ifndef LISTWINDOW_H
#define LISTWINDOW_H

#include <QDialog>

#include "../Projects/project.h"


namespace Ui {
    class ListWindow;
}

class ListWindow : public QDialog
{
        Q_OBJECT

    public:
        explicit ListWindow(QWidget *parent = 0);
        ~ListWindow();

        QString getListName() const;
        QString getListItemType() const;

        void setListName(const QString& listName);
        void setListItemType(const QString& itemType);

        void setProject(QSharedPointer<Tome::Project> project);

    public slots:
        void accept();

    private:
        Ui::ListWindow *ui;

        QSharedPointer<Tome::Project> project;

        bool validate();
};

#endif // LISTWINDOW_H
