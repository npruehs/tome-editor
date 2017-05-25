#include "newprojectwindow.h"
#include "ui_newprojectwindow.h"

#include <QFileDialog>
#include <QStandardPaths>

using namespace Tome;


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

    // Add record id types.
    this->ui->comboBoxRecordIdType->addItem(RecordIdType::toString(RecordIdType::Integer));
    this->ui->comboBoxRecordIdType->addItem(RecordIdType::toString(RecordIdType::Uuid));
    this->ui->comboBoxRecordIdType->addItem(RecordIdType::toString(RecordIdType::String));

    // Focus project name box.
    ui->lineEditName->setFocus();
}

NewProjectWindow::~NewProjectWindow()
{
    delete this->ui;
}

QString NewProjectWindow::getProjectName() const
{
    return ui->lineEditName->text();
}

QString NewProjectWindow::getProjectPath() const
{
    return ui->lineEditLocation->text();
}

RecordIdType::RecordIdType NewProjectWindow::getProjectRecordIdType() const
{
    return RecordIdType::fromString(this->ui->comboBoxRecordIdType->currentText());
}

void NewProjectWindow::on_comboBoxRecordIdType_currentTextChanged(const QString &text)
{
    Q_UNUSED(text)

    switch (this->getProjectRecordIdType())
    {
        case RecordIdType::Integer:
            this->ui->labelRecordIdTypeDescription->setText(tr("Automatically assign pesudo-random positive integer ids to all records."));
            break;

        case RecordIdType::String:
            this->ui->labelRecordIdTypeDescription->setText(tr("Manually assign string ids to all records."));
            break;

        case RecordIdType::Uuid:
            this->ui->labelRecordIdTypeDescription->setText(tr("Automatically assign Universally Unique Identifiers (UUIDs) to all records."));
            break;

        default:
            this->ui->labelRecordIdTypeDescription->clear();
    }
}

void NewProjectWindow::on_pushButtonBrowse_clicked()
{
    // Open folder browser dialog.
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
