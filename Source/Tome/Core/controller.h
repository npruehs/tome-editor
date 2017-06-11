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

    /**
     * @brief Main controller of Tome. Sets up and owns all other controllers and views.
     */
    class Controller : QObject
    {
            Q_OBJECT

        public:
            /**
             * @brief Constructs and sets up all controllers.
             * @param options Command-line options specifying which views to set up.
             */
            Controller(Tome::CommandLineOptions* options);
            ~Controller();

            /**
             * @brief Gets the controller for creating, loading and saving projects.
             * @return Controller for creating, loading and saving projects.
             */
            ProjectController& getProjectController() const;

            /**
             * @brief Gets the controller for performing undo-able commands.
             * @return Controller for performing undo-able commands.
             */
            UndoController& getUndoController() const;

            /**
             * @brief Gets the controller for adding and removing components.
             * @return Controller for adding and removing components.
             */
            ComponentsController& getComponentsController() const;

            /**
             * @brief Gets the controller for adding, updating and removing field definitions.
             * @return Controller for adding, updating and removing field definitions.
             */
            FieldDefinitionsController& getFieldDefinitionsController() const;

            /**
             * @brief Gets the controller for adding, updating and removing records.
             * @return Controller for adding, updating and removing records.
             */
            RecordsController& getRecordsController() const;

            /**
             * @brief Gets the controller for exporting records using templates.
             * @return Controller for exporting records using templates.
             */
            ExportController& getExportController() const;

            /**
             * @brief Gets the controller for accessing user settings.
             * @return Controller for accessing user settings.
             */
            SettingsController& getSettingsController() const;

            /**
             * @brief Gets the controller for runnings tasks on all records.
             * @return Controller for runnings tasks on all records.
             */
            TasksController& getTasksController() const;

            /**
             * @brief Gets the controller for adding, updating and removing custom types.
             * @return Controller for adding, updating and removing custom types.
             */
            TypesController& getTypesController() const;

            /**
             * @brief Gets the controller for finding usages of fields, records and custom types.
             * @return Controller for finding usages of fields, records and custom types.
             */
            FindUsagesController& getFindUsagesController() const;

            /**
             * @brief Gets the controller for finding specific records.
             * @return Controller for finding specific records.
             */
            FindRecordController& getFindRecordController() const;

            /**
             * @brief Gets the controller for validating type facets.
             * @return Controller for validating type facets.
             */
            FacetsController& getFacetsController() const;

            /**
             * @brief Gets the controller for importing record data.
             * @return Controller for importing record data.
             */
            ImportController& getImportController() const;

            /**
             * @brief Applies all command-line options and sets up the main window.
             * @return Zero, if all operations were successful, and another number otherwise.
             */
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
