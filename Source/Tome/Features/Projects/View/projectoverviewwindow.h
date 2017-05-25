#ifndef PROJECTOVERVIEWWINDOW_H
#define PROJECTOVERVIEWWINDOW_H

#include <QDialog>
#include <QListWidgetItem>

namespace Tome
{
    class ComponentsController;
    class ExportController;
    class FieldDefinitionsController;
    class ImportController;
    class ProjectController;
    class RecordsController;
    class TypesController;
}

namespace Ui
{
    class ProjectOverviewWindow;
}

/**
 * @brief Window for adding files to or removing files from the project.
 */
class ProjectOverviewWindow : public QDialog
{
        Q_OBJECT

    public:
        /**
         * @brief Constructs a new window for adding files to or removing files from the project.
         * @param projectController Controller for creating, loading and saving projects.
         * @param componentsController Controller for adding and removing components.
         * @param exportController Controller for exporting records using templates.
         * @param fieldDefinitionsController Controller for adding, updating and removing field definitions.
         * @param importController Controller for importing record data.
         * @param recordsController Controller for adding, updating and removing records.
         * @param typesController Controller for adding, updating and removing custom types.
         * @param parent Optional owner widget.
         */
        explicit ProjectOverviewWindow(Tome::ProjectController& projectController,
                                       Tome::ComponentsController& componentsController,
                                       Tome::ExportController& exportController,
                                       Tome::FieldDefinitionsController& fieldDefinitionsController,
                                       Tome::ImportController& importController,
                                       Tome::RecordsController& recordsController,
                                       Tome::TypesController& typesController,
                                       QWidget *parent = 0);
        ~ProjectOverviewWindow();

    protected:
        /**
         * @brief Updates the view, if necessary, and shows the window.
         * @param event Event for showing the window.
         */
        virtual void showEvent(QShowEvent* event) Q_DECL_OVERRIDE;

    private:
        Ui::ProjectOverviewWindow *ui;

        Tome::ComponentsController& componentsController;
        Tome::ExportController& exportController;
        Tome::FieldDefinitionsController& fieldDefinitionsController;
        Tome::ImportController& importController;
        Tome::ProjectController& projectController;
        Tome::RecordsController& recordsController;
        Tome::TypesController& typesController;

        QListWidgetItem* getSelectedListWidgetItem(QListWidget* listWidget);

        void navigateToSelectedFile(QListWidgetItem* item);

        void updateComponentData();
        void updateCustomTypeData();
        void updateExportTemplateData();
        void updateFieldDefinitionData();
        void updateImportTemplateData();
        void updateRecordData();

    private slots:
        void onAddExistingComponentsFileClicked(bool checked);
        void onAddExistingCustomTypesFileClicked(bool checked);
        void onAddExistingExportTemplateFileClicked(bool checked);
        void onAddExistingFieldDefinitionsFileClicked(bool checked);
        void onAddExistingImportTemplateFileClicked(bool checked);
        void onAddExistingRecordsFileClicked(bool checked);

        void onAddNewComponentsFileClicked(bool checked);
        void onAddNewCustomTypesFileClicked(bool checked);
        void onAddNewFieldDefinitionsFileClicked(bool checked);
        void onAddNewRecordsFileClicked(bool checked);

        void onNavigateToComponentsFileClicked(bool checked);
        void onNavigateToCustomTypesFileClicked(bool checked);
        void onNavigateToExportTemplateFileClicked(bool checked);
        void onNavigateToFieldDefinitionsFileClicked(bool checked);
        void onNavigateToImportTemplateFileClicked(bool checked);
        void onNavigateToRecordsFileClicked(bool checked);

        void onRemoveComponentsFileClicked(bool checked);
        void onRemoveCustomTypesFileClicked(bool checked);
        void onRemoveExportTemplateFileClicked(bool checked);
        void onRemoveFieldDefinitionsFileClicked(bool checked);
        void onRemoveImportTemplateFileClicked(bool checked);
        void onRemoveRecordsFileClicked(bool checked);
};

#endif // PROJECTOVERVIEWWINDOW_H
