#include "duplicaterecordwindow.h"
#include "ui_duplicaterecordwindow.h"

#include <QMessageBox>

DuplicateRecordWindow::DuplicateRecordWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DuplicateRecordWindow)
{
    ui->setupUi(this);
}

DuplicateRecordWindow::~DuplicateRecordWindow()
{
    delete this->ui;
}

QString DuplicateRecordWindow::getRecordId() const
{
    return this->ui->lineEdit->text();
}

void DuplicateRecordWindow::setDisallowedRecordIds(const QStringList disallowedRecordIds)
{
    this->disallowedRecordIds = disallowedRecordIds;
}

void DuplicateRecordWindow::setRecordId(const QString& id)
{
    this->ui->lineEdit->setText(id);
}

void DuplicateRecordWindow::accept()
{
    // Validate data.
    if (this->validate())
    {
        this->done(Accepted);
    }
}

void DuplicateRecordWindow::showEvent(QShowEvent* event)
{
    QDialog::showEvent(event);
    this->ui->lineEdit->setFocus();
}

bool DuplicateRecordWindow::validate()
{
    // Record ids must be unique.
    if (this->disallowedRecordIds.contains(this->getRecordId()))
    {
        QMessageBox::information(
                    this,
                    tr("Duplicate record id"),
                    tr("Please specify another id for the record."),
                    QMessageBox::Close,
                    QMessageBox::Close);
        return false;
    }

    return true;
}
