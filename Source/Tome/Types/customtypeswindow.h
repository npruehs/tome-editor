#ifndef CUSTOMTYPESWINDOW_H
#define CUSTOMTYPESWINDOW_H

#include <QMainWindow>

#include "customtypestablemodel.h"
#include "enumerationwindow.h"
#include "listwindow.h"
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
        void on_actionNew_List_triggered();

        void on_actionEdit_Custom_Type_triggered();
        void on_actionDelete_Custom_Type_triggered();

        void on_tableView_doubleClicked(const QModelIndex &index);

    private:
        Ui::CustomTypesWindow *ui;

        EnumerationWindow* enumerationWindow;
        ListWindow* listWindow;

        QSharedPointer<Tome::Project> project;
        QSharedPointer<Tome::CustomTypesTableModel> viewModel;

        int getSelectedTypeIndex() const;

        void editEnumeration(int index, QSharedPointer<Tome::CustomType> type);
        void editList(int index, QSharedPointer<Tome::CustomType> type);
};

#endif // CUSTOMTYPESWINDOW_H
