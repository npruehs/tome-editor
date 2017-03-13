#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QSharedPointer>

class MainWindow;

namespace Tome
{
    class CommandLineOptions;
    class ComponentsController;
    class ExportController;
    class FacetsController;
    class FieldDefinitionsController;
    class FindRecordController;
    class FindUsagesController;
    class ImportController;
    class Project;
    class ProjectController;
    class RecordsController;
    class SettingsController;
    class TasksController;
    class TypesController;
    class UndoController;

    class Controller : QObject
    {
            Q_OBJECT

        public:
            Controller(Tome::CommandLineOptions* options);
            ~Controller();

            ProjectController& getProjectController() const;
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
            ImportController& getImportController() const;

            int start();

        private slots:
            void onProjectChanged(QSharedPointer<Tome::Project> project);

        private:
            CommandLineOptions* options;

            ProjectController* projectController;
            UndoController* undoController;
            ComponentsController* componentsController;
            TypesController* typesController;
            FieldDefinitionsController* fieldDefinitionsController;
            RecordsController* recordsController;
            FacetsController* facetsController;
            ExportController* exportController;
            SettingsController* settingsController;
            TasksController* tasksController;
            FindUsagesController* findUsagesController;
            FindRecordController* findRecordController;
            ImportController* importController;

            MainWindow* mainWindow;
    };
}

#endif // CONTROLLER_H
