#ifndef PROJECTOVERVIEWWINDOW_H
#define PROJECTOVERVIEWWINDOW_H

#include <QDialog>

namespace Tome
{
    class Controller;
}

namespace Ui
{
    class ProjectOverviewWindow;
}

class ProjectOverviewWindow : public QDialog
{
        Q_OBJECT

    public:
        explicit ProjectOverviewWindow(Tome::Controller* controller, QWidget *parent = 0);
        ~ProjectOverviewWindow();

    protected:
        virtual void showEvent(QShowEvent* event);

    private:
        Ui::ProjectOverviewWindow *ui;

        Tome::Controller* controller;
};

#endif // PROJECTOVERVIEWWINDOW_H
