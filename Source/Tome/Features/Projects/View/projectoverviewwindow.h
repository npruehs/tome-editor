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

class ProjectOverviewWindow : public QDialog
{
        Q_OBJECT

    public:
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
