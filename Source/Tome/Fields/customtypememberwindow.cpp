#include "customtypememberwindow.h"
#include "ui_customtypememberwindow.h"

#include <QMessageBox>


CustomTypeMemberWindow::CustomTypeMemberWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CustomTypeMemberWindow)
{
    ui->setupUi(this);
}

CustomTypeMemberWindow::~CustomTypeMemberWindow()
{
    delete ui;
}

QString CustomTypeMemberWindow::getText() const
{
    return this->ui->lineEdit->text();
}

void CustomTypeMemberWindow::accept()
{
    // Validate data.
    if (this->validate())
    {
        this->done(Accepted);
    }
}

bool CustomTypeMemberWindow::validate()
{
    // Name must not be empty.
    if (this->getText().isEmpty())
    {
        QMessageBox::information(
                    this,
                    tr("Missing data"),
                    tr("Please specify a name for the type member."),
                    QMessageBox::Close,
                    QMessageBox::Close);
        return false;
    }

    return true;
}
