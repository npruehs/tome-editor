#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <stdexcept>

#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardPaths>
#include <QTextStream>
#include <QXmlStreamWriter>

#include "Fields/fielddefinitionsetserializer.h"
#include "Projects/project.h"
#include "Projects/projectserializer.h"
#include "Records/recordsetserializer.h"
#include "Util/pathutils.h"

using namespace Tome;

const QString MainWindow::FieldDefinitionFileExtension = ".tfields";
const QString MainWindow::ProjectFileExtension = ".tproj";
const QString MainWindow::RecordFileExtension = ".tdata";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    aboutWindow(0),
    fieldDefinitionsWindow(0),
    newProjectWindow(0)
{
    ui->setupUi(this);

    // Maximize window.
    this->showMaximized();

    // Can't access some functionality until project created or loaded.
    this->ui->actionSave_Project->setEnabled(false);
    this->ui->actionField_Definions->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionAbout_triggered()
{
    if (!this->aboutWindow)
    {
        this->aboutWindow = new AboutWindow(this);
    }

    this->showWindow(this->aboutWindow);
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::on_actionField_Definions_triggered()
{
    if (!this->fieldDefinitionsWindow)
    {
        this->fieldDefinitionsWindow = new FieldDefinitionsWindow(this->project, this);
    }

    this->showWindow(this->fieldDefinitionsWindow);
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

void MainWindow::on_actionOpen_Project_triggered()
{
    // Open file browser dialog.
    const QString projectFileName = QFileDialog::getOpenFileName(this,
                                                          tr("Open Project"),
                                                          QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
                                                          "Tome Project Files (*.tproj)");

    if (projectFileName.count() <= 0)
    {
        return;
    }

    // Open project file.
    QSharedPointer<QFile> projectFile = QSharedPointer<QFile>::create(projectFileName);
    QSharedPointer<QFileInfo> projectFileInfo = QSharedPointer<QFileInfo>::create(projectFileName);
    const QString projectPath = projectFileInfo->path();

    if (projectFile->open(QIODevice::ReadOnly))
    {
        // Load project from file.
        QSharedPointer<ProjectSerializer> projectSerializer =
                QSharedPointer<ProjectSerializer>::create();
        QSharedPointer<Project> project = QSharedPointer<Project>::create();
        project->path = projectPath;

        try
        {
             projectSerializer->deserialize(projectFile, project);
        }
        catch (const std::runtime_error& e)
        {
            QMessageBox::critical(
                        this,
                        tr("Unable to open project"),
                        tr("File could not be read:\r\n") + e.what(),
                        QMessageBox::Close,
                        QMessageBox::Close);
            return;
        }

        // Load field definition files.
        QSharedPointer<FieldDefinitionSetSerializer> fieldDefinitionSerializer =
                QSharedPointer<FieldDefinitionSetSerializer>::create();

        for (QVector<QSharedPointer<FieldDefinitionSet> >::iterator it = project->fieldDefinitionSets.begin();
             it != project->fieldDefinitionSets.end();
             ++it)
        {
            QSharedPointer<FieldDefinitionSet> fieldDefinitionSet = *it;

            // Open field definition file.
            const QString fullFieldDefinitionSetPath = combinePaths(projectPath, fieldDefinitionSet->name + FieldDefinitionFileExtension);
            QSharedPointer<QFile> fieldDefinitionFile = QSharedPointer<QFile>::create(fullFieldDefinitionSetPath);

            if (fieldDefinitionFile->open(QIODevice::ReadOnly))
            {
                try
                {
                     fieldDefinitionSerializer->deserialize(fieldDefinitionFile, fieldDefinitionSet);
                }
                catch (const std::runtime_error& e)
                {
                    QMessageBox::critical(
                                this,
                                tr("Unable to open project"),
                                tr("File could not be read:\r\n") + e.what(),
                                QMessageBox::Close,
                                QMessageBox::Close);
                    return;
                }
            }
            else
            {
                QMessageBox::critical(
                            this,
                            tr("Unable to open project"),
                            tr("File could not be read:\r\n") + fullFieldDefinitionSetPath,
                            QMessageBox::Close,
                            QMessageBox::Close);
                return;
            }
        }

        // Load record files.
        QSharedPointer<RecordSetSerializer> recordSetSerializer =
                QSharedPointer<RecordSetSerializer>::create();

        for (QVector<QSharedPointer<RecordSet> >::iterator it = project->recordSets.begin();
             it != project->recordSets.end();
             ++it)
        {
            QSharedPointer<RecordSet> recordSet = *it;

            // Open record file.
            const QString fullRecordSetPath = combinePaths(projectPath, recordSet->name + RecordFileExtension);
            QSharedPointer<QFile> recordFile = QSharedPointer<QFile>::create(fullRecordSetPath);

            if (recordFile->open(QIODevice::ReadOnly))
            {
                try
                {
                     recordSetSerializer->deserialize(recordFile, recordSet);
                }
                catch (const std::runtime_error& e)
                {
                    QMessageBox::critical(
                                this,
                                tr("Unable to open project"),
                                tr("File could not be read:\r\n") + e.what(),
                                QMessageBox::Close,
                                QMessageBox::Close);
                    return;
                }
            }
            else
            {
                QMessageBox::critical(
                            this,
                            tr("Unable to open project"),
                            tr("File could not be read:\r\n") + fullRecordSetPath,
                            QMessageBox::Close,
                            QMessageBox::Close);
                return;
            }
        }

        // Set project reference.
        this->setProject(project);
    }
    else
    {
        QMessageBox::critical(
                    this,
                    tr("Unable to open project"),
                    tr("File could not be read:\r\n") + projectFileName,
                    QMessageBox::Close,
                    QMessageBox::Close);
        return;
    }
}

void MainWindow::on_actionSave_Project_triggered()
{
    this->saveProject(this->project);
}

void MainWindow::createNewProject(const QString &projectName, const QString &projectPath)
{
    // Create new project.
    QSharedPointer<Project> newProject = QSharedPointer<Project>::create();
    newProject->name = projectName;
    newProject->path = projectPath;

    // Create field definition set.
    QSharedPointer<FieldDefinitionSet> fieldDefinitionSet =
            QSharedPointer<FieldDefinitionSet>::create();
    fieldDefinitionSet->name = projectName;
    newProject->fieldDefinitionSets.push_back(fieldDefinitionSet);

    // Create record set.
    QSharedPointer<RecordSet> recordSet =
            QSharedPointer<RecordSet>::create();
    recordSet->name = projectName;
    newProject->recordSets.push_back(recordSet);

    // Add dummy data.
    QSharedPointer<FieldDefinition> fieldDefinition =
            QSharedPointer<FieldDefinition>::create();
    fieldDefinition->defaultValue = "123";
    fieldDefinition->description = "Maximum health value of the entity.";
    fieldDefinition->displayName = "Maximum Health";
    fieldDefinition->fieldType = FieldType::Int;
    fieldDefinition->id = "MaximumHealth";
    fieldDefinitionSet->fieldDefinitions.push_back(fieldDefinition);

    QSharedPointer<Record> record = QSharedPointer<Record>::create();
    record->id = "Knight";
    record->fieldValues["MaximumHealth"] = "70";
    recordSet->records.push_back(record);

    // Write project files.
    this->saveProject(newProject);

    // Set project reference.
    this->setProject(newProject);
}

void MainWindow::saveProject(QSharedPointer<Project> project)
{
    QString& projectName = project->name;
    QString& projectPath = project->path;

    QSharedPointer<ProjectSerializer> projectSerializer =
            QSharedPointer<ProjectSerializer>::create();

    // Build file name.
    const QString projectFileName = projectName + ProjectFileExtension;
    const QString fullProjectPath = combinePaths(projectPath, projectFileName);

    // Write project file.
    QSharedPointer<QFile> projectFile = QSharedPointer<QFile>::create(fullProjectPath);

    if (projectFile->open(QIODevice::ReadWrite))
    {
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
        return;
    }

    // Write field definition sets.
    QSharedPointer<FieldDefinitionSetSerializer> fieldDefinitionSetSerializer =
            QSharedPointer<FieldDefinitionSetSerializer>::create();

    for (QVector<QSharedPointer<FieldDefinitionSet> >::iterator it = project->fieldDefinitionSets.begin();
         it != project->fieldDefinitionSets.end();
         ++it)
    {
        QSharedPointer<FieldDefinitionSet> fieldDefinitionSet = *it;

        // Build file name.
        const QString fieldDefinitionSetFileName = fieldDefinitionSet->name + FieldDefinitionFileExtension;
        const QString fullFieldDefinitionSetPath = combinePaths(projectPath, fieldDefinitionSetFileName);

        // Write file.
        QSharedPointer<QFile> fieldDefinitionSetFile = QSharedPointer<QFile>::create(fullFieldDefinitionSetPath);

        if (fieldDefinitionSetFile->open(QIODevice::ReadWrite))
        {
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
    }


    // Write record sets.
    QSharedPointer<Tome::RecordSetSerializer> recordSetSerializer =
            QSharedPointer<Tome::RecordSetSerializer>::create();

    for (QVector<QSharedPointer<RecordSet> >::iterator it = project->recordSets.begin();
         it != project->recordSets.end();
         ++it)
    {
        QSharedPointer<RecordSet> recordSet = *it;

        // Build file name.
        const QString recordSetFileName = recordSet->name + RecordFileExtension;
        const QString fullRecordSetPath = Tome::combinePaths(projectPath, recordSetFileName);

        // Write file.
        QSharedPointer<QFile> recordSetFile = QSharedPointer<QFile>::create(fullRecordSetPath);

        if (recordSetFile->open(QIODevice::ReadWrite))
        {
            recordSetSerializer->serialize(recordSetFile, recordSet);
        }
    }
}

void MainWindow::setProject(QSharedPointer<Project> project)
{
    this->project = project;

    // Enable project-specific buttons.
    this->ui->actionSave_Project->setEnabled(true);
    this->ui->actionField_Definions->setEnabled(true);
}

void MainWindow::showWindow(QWidget* widget)
{
    widget->show();
    widget->raise();
    widget->activateWindow();
}
