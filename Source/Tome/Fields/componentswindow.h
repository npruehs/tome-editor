#ifndef COMPONENTSWINDOW_H
#define COMPONENTSWINDOW_H

#include <QMainWindow>

#include "componentwindow.h"
#include "componentsitemmodel.h"
#include "../Projects/project.h"

namespace Ui {
    class ComponentsWindow;
}

class ComponentsWindow : public QMainWindow
{
        Q_OBJECT

    public:
        explicit ComponentsWindow(QSharedPointer<Tome::Project> project, QWidget *parent = 0);
        ~ComponentsWindow();

    private slots:
        void on_actionNew_Component_triggered();
        void on_actionDelete_Component_triggered();

    private:
        Ui::ComponentsWindow *ui;

        ComponentWindow* componentWindow;

        QSharedPointer<Tome::Project> project;
        QSharedPointer<Tome::ComponentsItemModel> viewModel;
};

#endif // COMPONENTSWINDOW_H
