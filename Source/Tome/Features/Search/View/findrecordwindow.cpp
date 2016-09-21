#include "findrecordwindow.h"
#include "ui_findrecordwindow.h"

FindRecordWindow::FindRecordWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FindRecordWindow)
{
    ui->setupUi(this);
}

FindRecordWindow::~FindRecordWindow()
{
    delete ui;
}

QString FindRecordWindow::getSearchPattern() const
{
    return this->ui->lineEdit->text();
}
