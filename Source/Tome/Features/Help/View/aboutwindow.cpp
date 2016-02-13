#include "aboutwindow.h"
#include "ui_aboutwindow.h"

AboutWindow::AboutWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutWindow)
{
    ui->setupUi(this);

    // Show version number.
    this->ui->labelVersion->setText("Version " + QApplication::instance()->applicationVersion());
}

AboutWindow::~AboutWindow()
{
    delete this->ui;
}
