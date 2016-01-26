#ifndef CUSTOMTYPESWINDOW_H
#define CUSTOMTYPESWINDOW_H

#include <QMainWindow>

#include "customtypesitemmodel.h"
#include "enumerationwindow.h"
#include "../Projects/project.h"

namespace Ui {
    class CustomTypesWindow;
}

class CustomTypesWindow : public QMainWindow
{
        Q_OBJECT

    public:
        explicit CustomTypesWindow(QSharedPointer<Tome::Project> project, QWidget *parent = 0);
        ~CustomTypesWindow();

    private slots:
        void on_actionNew_Custom_Type_triggered();
        void on_actionEdit_Custom_Type_triggered();
        void on_actionDelete_Custom_Type_triggered();

        void on_listView_doubleClicked(const QModelIndex &index);

    private:
        Ui::CustomTypesWindow *ui;

        EnumerationWindow* enumerationWindow;

        QSharedPointer<Tome::Project> project;
        QSharedPointer<Tome::CustomTypesItemModel> viewModel;
};

#endif // CUSTOMTYPESWINDOW_H
