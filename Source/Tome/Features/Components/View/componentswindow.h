#ifndef COMPONENTSWINDOW_H
#define COMPONENTSWINDOW_H

#include <QMainWindow>

#include "componentwindow.h"
#include "../Controller/componentscontroller.h"

namespace Ui {
    class ComponentsWindow;
}


class ComponentsWindow : public QMainWindow
{
        Q_OBJECT

    public:
        explicit ComponentsWindow(Tome::ComponentsController& controller, QWidget *parent = 0);
        ~ComponentsWindow();

    private slots:
        void on_actionNew_Component_triggered();
        void on_actionDelete_Component_triggered();

    private:
        Ui::ComponentsWindow *ui;

        Tome::ComponentsController& controller;

        ComponentWindow* componentWindow;
};

#endif // COMPONENTSWINDOW_H
