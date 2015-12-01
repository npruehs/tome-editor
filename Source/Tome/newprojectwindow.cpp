#include "newprojectwindow.h"
#include "ui_newprojectwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QStandardPaths>
#include <QTextStream>
#include <QXmlStreamWriter>

#include "tomeproject.h"
#include "Projects/tomeprojectserializer.h"
#include "Fields/fielddefinitionfileserializer.h"

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

void NewProjectWindow::on_buttonBox_accepted()
{
    // Build project file name.
    const QString projectName = ui->lineEditName->text();
    const QString projectPath = ui->lineEditLocation->text();
    const QString projectFileName = projectName + ".tproj";
    const QString fieldDefinitionFileName = projectName + ".tfields";
    const QString fullProjectPath = projectPath + "/" + projectFileName;
    const QString fullFieldDefinitionFilePath = projectPath + "/" + fieldDefinitionFileName;

    // Create new project.
    QSharedPointer<Tome::TomeProject> project =
            QSharedPointer<Tome::TomeProject>::create();
    project->name = projectName;

    // Create field definition file.
    QSharedPointer<Tome::FieldDefinitionFile> fieldDefinitionFileData =
            QSharedPointer<Tome::FieldDefinitionFile>::create();
    fieldDefinitionFileData->path = fieldDefinitionFileName;

    // Add field definition file to project.
    project->fieldDefinitionFiles.push_back(fieldDefinitionFileData);

    // Write project file.
    QSharedPointer<QFile> projectFile = QSharedPointer<QFile>::create(fullProjectPath);

    if (projectFile->open(QIODevice::ReadWrite))
    {
        QSharedPointer<Tome::TomeProjectSerializer> projectSerializer =
                QSharedPointer<Tome::TomeProjectSerializer>::create();
        projectSerializer->serialize(projectFile, project);
    }
    else
    {
        QMessageBox::critical(
                    this,
                    tr("Unable to create project"),
                    tr("Destination file could not be written:\r\n") + fullProjectPath,
                    QMessageBox::Close,
                    QMessageBox::Close);

    }

    // Write field definition file.
    QSharedPointer<QFile> fieldDefinitionFile = QSharedPointer<QFile>::create(fullFieldDefinitionFilePath);

    if (fieldDefinitionFile->open(QIODevice::ReadWrite))
    {
        QSharedPointer<Tome::FieldDefinitionFileSerializer> fieldDefinitionFileSerializer =
                QSharedPointer<Tome::FieldDefinitionFileSerializer>::create();
        fieldDefinitionFileSerializer->serialize(fieldDefinitionFile, fieldDefinitionFileData);
    }
    else
    {
        QMessageBox::critical(
                    this,
                    tr("Unable to create project"),
                    tr("Destination file could not be written:\r\n") + fullFieldDefinitionFilePath,
                    QMessageBox::Close,
                    QMessageBox::Close);

    }
}
