#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>

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
        void on_buttonBox_accepted();
        void on_buttonBox_rejected();

        void onNetworkReply(QNetworkReply* reply);

    private:
        Ui::MainWindow *ui;

        QNetworkAccessManager* networkAccessManager;
};

#endif // MAINWINDOW_H
