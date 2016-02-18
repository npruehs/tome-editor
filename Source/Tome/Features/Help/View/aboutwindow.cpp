#include "aboutwindow.h"
#include "ui_aboutwindow.h"

AboutWindow::AboutWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutWindow)
{
    ui->setupUi(this);

    // Show version number.
    const QString version = "Version " + QApplication::instance()->applicationVersion() + " (" + APP_VERSION_NAME + ")";
    this->ui->labelVersion->setText(version);
}

AboutWindow::~AboutWindow()
{
    delete this->ui;
}
