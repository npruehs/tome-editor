#ifndef NEWPROJECTWINDOW_H
#define NEWPROJECTWINDOW_H

#include <QDialog>
#include <QString>

namespace Ui {
    class NewProjectWindow;
}

class NewProjectWindow : public QDialog
{
        Q_OBJECT

    public:
        explicit NewProjectWindow(QWidget *parent = 0);
        ~NewProjectWindow();

        QString getProjectName() const;
        QString getProjectPath() const;

    private slots:
        void on_pushButtonBrowse_clicked();

    private:
        Ui::NewProjectWindow *ui;
};

#endif // NEWPROJECTWINDOW_H
