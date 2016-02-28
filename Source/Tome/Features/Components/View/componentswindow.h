#ifndef COMPONENTSWINDOW_H
#define COMPONENTSWINDOW_H

#include <QMainWindow>

class ComponentWindow;

namespace Ui {
    class ComponentsWindow;
}

namespace Tome
{
    class ComponentsController;
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
