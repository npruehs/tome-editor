#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QXmlStreamWriter>

#include "Fields/fielddefinitionsetserializer.h"
#include "Projects/project.h"
#include "Projects/projectserializer.h"
#include "Records/recordsetserializer.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    newProjectWindow(0),
    aboutWindow(0)

{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionNew_Project_triggered()
{
    if (!this->newProjectWindow)
    {
        this->newProjectWindow = new NewProjectWindow(this);
    }

    int result = this->newProjectWindow->exec();

    if (result == QDialog::Accepted)
    {
        const QString projectName = this->newProjectWindow->getProjectName();
        const QString projectPath = this->newProjectWindow->getProjectPath();

        this->createNewProject(projectName, projectPath);
    }
}

void MainWindow::on_actionAbout_triggered()
{
    if (!this->aboutWindow)
    {
        this->aboutWindow = new AboutWindow(this);
    }

    this->aboutWindow->show();
    this->aboutWindow->raise();
    this->aboutWindow->activateWindow();
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::createNewProject(QString projectName, QString projectPath)
{
    // Build file names.
    const QString projectFileName = projectName + ".tproj";
    const QString fieldDefinitionSetName = projectName + ".tfields";
    const QString recordSetName = projectName + ".tdata";

    const QString fullProjectPath = projectPath + "/" + projectFileName;
    const QString fullFieldDefinitionSetPath = projectPath + "/" + fieldDefinitionSetName;
    const QString fullRecordSetPath = projectPath + "/" + recordSetName;

    // Create new project.
    QSharedPointer<Tome::Project> newProject = QSharedPointer<Tome::Project>::create();
    newProject->name = projectName;

    // Create field definition set.
    QSharedPointer<Tome::FieldDefinitionSet> fieldDefinitionSet =
            QSharedPointer<Tome::FieldDefinitionSet>::create();
    fieldDefinitionSet->name = fieldDefinitionSetName;
    newProject->fieldDefinitionSets.push_back(fieldDefinitionSet);

    // Create record set.
    QSharedPointer<Tome::RecordSet> recordSet =
            QSharedPointer<Tome::RecordSet>::create();
    recordSet->name = recordSetName;
    newProject->recordSets.push_back(recordSet);

    // Add dummy data.
    QSharedPointer<Tome::FieldDefinition> fieldDefinition =
            QSharedPointer<Tome::FieldDefinition>::create();
    fieldDefinition->defaultValue = "123";
    fieldDefinition->description = "Maximum health value of the entity.";
    fieldDefinition->displayName = "Maximum Health";
    fieldDefinition->fieldType = Tome::FieldType::Int;
    fieldDefinition->id = "MaximumHealth";
    fieldDefinitionSet->fieldDefinitions.push_back(fieldDefinition);

    QSharedPointer<Tome::Record> record = QSharedPointer<Tome::Record>::create();
    record->id = "Knight";
    record->fieldValues["MaximumHealth"] = "70";
    recordSet->records.push_back(record);

    // Write project file.
    QSharedPointer<QFile> projectFile = QSharedPointer<QFile>::create(fullProjectPath);

    if (projectFile->open(QIODevice::ReadWrite))
    {
        QSharedPointer<Tome::ProjectSerializer> projectSerializer =
                QSharedPointer<Tome::ProjectSerializer>::create();
        projectSerializer->serialize(projectFile, newProject);
    }
    else
    {
        QMessageBox::critical(
                    this,
                    tr("Unable to create project"),
                    tr("Destination file could not be written:\r\n") + fullProjectPath,
                    QMessageBox::Close,
                    QMessageBox::Close);
        return;
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
        return;
    }

    // Write record set.
    QSharedPointer<QFile> recordSetFile = QSharedPointer<QFile>::create(fullRecordSetPath);

    if (recordSetFile->open(QIODevice::ReadWrite))
    {
        QSharedPointer<Tome::RecordSetSerializer> recordSetSerializer =
                QSharedPointer<Tome::RecordSetSerializer>::create();
        recordSetSerializer->serialize(recordSetFile, recordSet);
    }

    // Set project reference.
    this->project = newProject;
}
