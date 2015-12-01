#include "newprojectwindow.h"
#include "ui_newprojectwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QStandardPaths>
#include <QTextStream>
#include <QXmlStreamWriter>

#include "project.h"
#include "Projects/projectserializer.h"
#include "Fields/fielddefinitionsetserializer.h"

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
    const QString fieldDefinitionSetName = projectName + ".tfields";
    const QString fullProjectPath = projectPath + "/" + projectFileName;
    const QString fullFieldDefinitionSetPath = projectPath + "/" + fieldDefinitionSetName;

    // Create new project.
    QSharedPointer<Tome::Project> project =
            QSharedPointer<Tome::Project>::create();
    project->name = projectName;

    // Create field definition set.
    QSharedPointer<Tome::FieldDefinitionSet> fieldDefinitionSet =
            QSharedPointer<Tome::FieldDefinitionSet>::create();
    fieldDefinitionSet->name = fieldDefinitionSetName;
    project->fieldDefinitionSets.push_back(fieldDefinitionSet);

    // Write project file.
    QSharedPointer<QFile> projectFile = QSharedPointer<QFile>::create(fullProjectPath);

    if (projectFile->open(QIODevice::ReadWrite))
    {
        QSharedPointer<Tome::ProjectSerializer> projectSerializer =
                QSharedPointer<Tome::ProjectSerializer>::create();
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

    // Write field definition set.
    QSharedPointer<QFile> fieldDefinitionSetFile = QSharedPointer<QFile>::create(fullFieldDefinitionSetPath);

    if (fieldDefinitionSetFile->open(QIODevice::ReadWrite))
    {
        QSharedPointer<Tome::FieldDefinitionSetSerializer> fieldDefinitionSetSerializer =
                QSharedPointer<Tome::FieldDefinitionSetSerializer>::create();
        fieldDefinitionSetSerializer->serialize(fieldDefinitionSetFile, fieldDefinitionSet);
    }
    else
    {
        QMessageBox::critical(
                    this,
                    tr("Unable to create project"),
                    tr("Destination file could not be written:\r\n") + fullFieldDefinitionSetPath,
                    QMessageBox::Close,
                    QMessageBox::Close);

    }
}
