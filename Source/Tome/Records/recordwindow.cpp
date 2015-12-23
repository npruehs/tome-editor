#include "recordwindow.h"
#include "ui_recordwindow.h"

#include <QMessageBox>


RecordWindow::RecordWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RecordWindow)
{
    ui->setupUi(this);
}

RecordWindow::~RecordWindow()
{
    delete ui;
}

void RecordWindow::accept()
{
    // Validate data.
    if (this->validate())
    {
        this->done(Accepted);
    }
}

QString RecordWindow::getRecordDisplayName() const
{
    return this->ui->lineEditDisplayName->text();
}

QString RecordWindow::getRecordId() const
{
    return this->ui->lineEditId->text();
}

void RecordWindow::setRecordDisplayName(const QString& displayName)
{
    this->ui->lineEditDisplayName->setText(displayName);
}

void RecordWindow::setRecordId(const QString& id)
{
    this->ui->lineEditId->setText(id);
}

void RecordWindow::on_lineEditDisplayName_textEdited(const QString& displayName)
{
    this->setRecordId(displayName);
}

bool RecordWindow::validate()
{
    // Id must not be empty.
    if (this->getRecordId().size() == 0)
    {
        QMessageBox::information(
                    this,
                    tr("Missing data"),
                    tr("Please specify an id for the record."),
                    QMessageBox::Close,
                    QMessageBox::Close);
        return false;
    }

    // Display name must not be empty.
    if (this->getRecordDisplayName().size() == 0)
    {
        QMessageBox::information(
                    this,
                    tr("Missing data"),
                    tr("Please specify a name for the record."),
                    QMessageBox::Close,
                    QMessageBox::Close);
        return false;
    }

    return true;
}
