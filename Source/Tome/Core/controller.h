#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QSharedPointer>

class MainWindow;

namespace Tome
{
    class CommandLineOptions;
    class ComponentsController;
    class ExportController;
    class FieldDefinitionsController;
    class FindUsagesController;
    class Project;
    class RecordsController;
    class SettingsController;
    class TasksController;
    class TypesController;

    class Controller : public QObject
    {
        Q_OBJECT

        public:
            Controller(Tome::CommandLineOptions* options);
            ~Controller();

            ComponentsController& getComponentsController();
            FieldDefinitionsController& getFieldDefinitionsController();
            RecordsController& getRecordsController();
            ExportController& getExportController();
            SettingsController& getSettingsController();
            TasksController& getTasksController();
            TypesController& getTypesController();
            FindUsagesController& getFindUsagesController();

            int start();

            void createProject(const QString& projectName, const QString& projectPath);
            const QString getFullProjectPath() const;
            const QString getProjectName() const;
            const QString getProjectPath() const;
            bool isProjectLoaded() const;
            void openProject(const QString& projectFileName);
            void saveProject();

        signals:
            void projectChanged(QSharedPointer<Tome::Project> project);

        private:
            static const QString FieldDefinitionFileExtension;
            static const QString ProjectFileExtension;
            static const QString RecordExportComponentDelimiterExtension;
            static const QString RecordExportComponentTemplateExtension;
            static const QString RecordExportListTemplateExtension;
            static const QString RecordExportListItemTemplateExtension;
            static const QString RecordExportListItemDelimiterExtension;
            static const QString RecordExportMapTemplateExtension;
            static const QString RecordExportMapItemTemplateExtension;
            static const QString RecordExportMapItemDelimiterExtension;
            static const QString RecordExportRecordFileTemplateExtension;
            static const QString RecordExportRecordTemplateExtension;
            static const QString RecordExportRecordDelimiterExtension;
            static const QString RecordExportFieldValueTemplateExtension;
            static const QString RecordExportFieldValueDelimiterExtension;
            static const QString RecordFileExtension;

            CommandLineOptions* options;

            QSharedPointer<Project> project;

            ComponentsController* componentsController;
            FieldDefinitionsController* fieldDefinitionsController;
            TypesController* typesController;
            RecordsController* recordsController;
            ExportController* exportController;
            SettingsController* settingsController;
            TasksController* tasksController;
            FindUsagesController* findUsagesController;

            MainWindow* mainWindow;

            const QString getFullProjectPath(QSharedPointer<Project> project) const;
            void saveProject(QSharedPointer<Project> project);
            void setProject(QSharedPointer<Project> project);
            QString readFile(const QString& path, const QString& fileName);
    };
}

#endif // CONTROLLER_H
