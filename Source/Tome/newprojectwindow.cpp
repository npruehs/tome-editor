#include "newprojectwindow.h"
#include "ui_newprojectwindow.h"

#include <QFileDialog>
#include <QStandardPaths>

NewProjectWindow::NewProjectWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewProjectWindow)
{
    ui->setupUi(this);

    // Set initial project name.
    ui->lineEditName->setText(tr("Another Tome Project"));

    // Set initial project folder.
    const QString documentsFolder = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    ui->lineEditLocation->setText(documentsFolder);
}

NewProjectWindow::~NewProjectWindow()
{
    delete ui;
}

void NewProjectWindow::on_pushButtonBrowse_clicked()
{
    // Open folde browser dialog.
    QString newFolder = QFileDialog::getExistingDirectory(this,
                                                    tr("Open Directory"),
                                                    ui->lineEditLocation->text(),
                                                    QFileDialog::ShowDirsOnly);

    if (newFolder.count() <= 0)
    {
        return;
    }

    // Update UI.
    ui->lineEditLocation->setText(newFolder);
}
