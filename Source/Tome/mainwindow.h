#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSharedPointer>

#include "Projects/newprojectwindow.h"
#include "Projects/project.h"
#include "Help/aboutwindow.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

    private slots:
        void on_actionAbout_triggered();
        void on_actionExit_triggered();
        void on_actionNew_Project_triggered();
        void on_actionOpen_Project_triggered();
        void on_actionSave_Project_triggered();

    private:
        Ui::MainWindow *ui;

        QSharedPointer<Tome::Project> project;

        NewProjectWindow *newProjectWindow;
        AboutWindow *aboutWindow;

        void createNewProject(const QString& name, const QString& path);
        void saveProject(QSharedPointer<Tome::Project> project);
        void setProject(QSharedPointer<Tome::Project> project);
};

#endif // MAINWINDOW_H
