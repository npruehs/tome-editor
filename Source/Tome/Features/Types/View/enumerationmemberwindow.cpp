#include "enumerationmemberwindow.h"
#include "ui_enumerationmemberwindow.h"

#include <QMessageBox>


EnumerationMemberWindow::EnumerationMemberWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EnumerationMemberWindow)
{
    ui->setupUi(this);
}

EnumerationMemberWindow::~EnumerationMemberWindow()
{
    delete this->ui;
}

QString EnumerationMemberWindow::getText() const
{
    return this->ui->lineEdit->text();
}

void EnumerationMemberWindow::accept()
{
    // Validate data.
    if (this->validate())
    {
        this->done(Accepted);
    }
}

bool EnumerationMemberWindow::validate()
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

    // Name must not contain member seperator.
    if (this->getText().contains(";"))
    {
        QMessageBox::information(
                    this,
                    tr("Invalid data"),
                    tr("Type member names must not contain any special characters."),
                    QMessageBox::Close,
                    QMessageBox::Close);
        return false;
    }

    return true;
}
