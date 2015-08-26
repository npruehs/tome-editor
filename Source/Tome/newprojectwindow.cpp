#include "newprojectwindow.h"
#include "ui_newprojectwindow.h"

#include <QStandardPaths>

NewProjectWindow::NewProjectWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewProjectWindow)
{
    ui->setupUi(this);

    // Set initial project name.
    ui->lineEditName->setText("Another Tome Project");

    // Set initial project folder.
    const QString documentsFolder = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    ui->lineEditLocation->setText(documentsFolder);
}

NewProjectWindow::~NewProjectWindow()
{
    delete ui;
}
