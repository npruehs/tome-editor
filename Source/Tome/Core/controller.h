#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QSharedPointer>

class MainWindow;

namespace Tome
{
    class CommandLineOptions;
    class ComponentSet;
    class ComponentsController;
    class CustomTypeSet;
    class ExportController;
    class FacetsController;
    class FieldDefinitionSet;
    class FieldDefinitionsController;
    class FindRecordController;
    class FindUsagesController;
    class Project;
    class RecordExportTemplate;
    class RecordSet;
    class RecordSetSerializer;
    class RecordsController;
    class SettingsController;
    class TasksController;
    class TypesController;
    class UndoController;

    class Controller : public QObject
    {
        Q_OBJECT

        public:
            Controller(Tome::CommandLineOptions* options);
            ~Controller();

            UndoController& getUndoController() const;
            ComponentsController& getComponentsController() const;
            FieldDefinitionsController& getFieldDefinitionsController() const;
            RecordsController& getRecordsController() const;
            ExportController& getExportController() const;
            SettingsController& getSettingsController() const;
            TasksController& getTasksController() const;
            TypesController& getTypesController() const;
            FindUsagesController& getFindUsagesController() const;
            FindRecordController& getFindRecordController() const;
            FacetsController& getFacetsController() const;

            int start();

            QString buildFullFilePath(QString filePath, QString projectPath, QString desiredExtension) const;
            void createProject(const QString& projectName, const QString& projectPath);
            const QString getFullProjectPath() const;
            const QString getProjectName() const;
            const QString getProjectPath() const;
            bool getProjectIgnoreReadOnly() const;
            bool isProjectLoaded() const;
            void loadComponentSet(const QString& projectPath, ComponentSet& componentSet) const;
            void loadCustomTypeSet(const QString& projectPath, CustomTypeSet& customTypeSet) const;
            void loadExportTemplate(const QString& projectPath, RecordExportTemplate& exportTemplate) const;
            void loadFieldDefinitionSet(const QString& projectPath, FieldDefinitionSet& fieldDefinitionSet) const;
            void loadRecordSet(const QString& projectPath, RecordSet& recordSet) const;
            void openProject(const QString& projectFileName);
            void saveProject() const;

            static const QString ComponentFileExtension;
            static const QString FieldDefinitionFileExtension;
            static const QString ProjectFileExtension;
            static const QString RecordExportComponentDelimiterExtension;
            static const QString RecordExportComponentTemplateExtension;
            static const QString RecordExportFieldValueTemplateExtension;
            static const QString RecordExportFieldValueDelimiterExtension;
            static const QString RecordExportListTemplateExtension;
            static const QString RecordExportListItemTemplateExtension;
            static const QString RecordExportListItemDelimiterExtension;
            static const QString RecordExportMapTemplateExtension;
            static const QString RecordExportMapItemTemplateExtension;
            static const QString RecordExportMapItemDelimiterExtension;
            static const QString RecordExportRecordFileTemplateExtension;
            static const QString RecordExportRecordTemplateExtension;
            static const QString RecordExportRecordDelimiterExtension;
            static const QString RecordExportTemplateFileExtension;
            static const QString RecordFileExtension;
            static const QString TypeFileExtension;

        signals:
            void progressChanged(const QString title, const QString text, const int currentValue, const int maximumValue);
            void projectChanged(QSharedPointer<Tome::Project> project);

        private slots:
            void onProgressChanged(const QString title, const QString text, const int currentValue, const int maximumValue);

        private:
            CommandLineOptions* options;

            QSharedPointer<Project> project;

            UndoController* undoController;
            ComponentsController* componentsController;
            FieldDefinitionsController* fieldDefinitionsController;
            TypesController* typesController;
            RecordsController* recordsController;
            ExportController* exportController;
            SettingsController* settingsController;
            FacetsController* facetsController;
            TasksController* tasksController;
            FindUsagesController* findUsagesController;
            FindRecordController* findRecordController;

            RecordSetSerializer* recordSetSerializer;

            MainWindow* mainWindow;

            const QString getFullProjectPath(QSharedPointer<Project> project) const;
            void saveProject(QSharedPointer<Project> project) const;
            void setProject(QSharedPointer<Project> project);
            QString readFile(const QString& fullPath) const;
    };
}

#endif // CONTROLLER_H
