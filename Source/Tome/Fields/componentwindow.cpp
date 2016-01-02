#include "componentwindow.h"
#include "ui_componentwindow.h"

#include <QMessageBox>


ComponentWindow::ComponentWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ComponentWindow)
{
    ui->setupUi(this);
}

ComponentWindow::~ComponentWindow()
{
    delete ui;
}

QString ComponentWindow::getComponentName() const
{
    return this->ui->lineEdit->text();
}

void ComponentWindow::accept()
{
    // Validate data.
    if (this->validate())
    {
        this->done(Accepted);
    }
}

bool ComponentWindow::validate()
{
    // Name must not be empty.
    if (this->getComponentName().isEmpty())
    {
        QMessageBox::information(
                    this,
                    tr("Missing data"),
                    tr("Please specify a name for the component."),
                    QMessageBox::Close,
                    QMessageBox::Close);
        return false;
    }

    return true;
}
