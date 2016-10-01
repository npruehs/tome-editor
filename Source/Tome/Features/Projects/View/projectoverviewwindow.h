#ifndef PROJECTOVERVIEWWINDOW_H
#define PROJECTOVERVIEWWINDOW_H

#include <QDialog>
#include <QListWidgetItem>

namespace Tome
{
    class Controller;
}

namespace Ui
{
    class ProjectOverviewWindow;
}

class ProjectOverviewWindow : public QDialog
{
        Q_OBJECT

    public:
        explicit ProjectOverviewWindow(Tome::Controller* controller, QWidget *parent = 0);
        ~ProjectOverviewWindow();

    protected:
        virtual void showEvent(QShowEvent* event);

    private:
        Ui::ProjectOverviewWindow *ui;

        Tome::Controller* controller;

        QListWidgetItem* getSelectedListWidgetItem(QListWidget* listWidget);

        void navigateToSelectedFile(QListWidgetItem* item);

        void updateComponentData();
        void updateCustomTypeData();
        void updateExportTemplateData();
        void updateFieldDefinitionData();
        void updateRecordData();

    private slots:
        void onAddExistingComponentsFileClicked(bool checked);
        void onAddExistingCustomTypesFileClicked(bool checked);
        void onAddExistingExportTemplateFileClicked(bool checked);
        void onAddExistingFieldDefinitionsFileClicked(bool checked);
        void onAddExistingRecordsFileClicked(bool checked);

        void onAddNewComponentsFileClicked(bool checked);
        void onAddNewCustomTypesFileClicked(bool checked);
        void onAddNewFieldDefinitionsFileClicked(bool checked);
        void onAddNewRecordsFileClicked(bool checked);

        void onNavigateToComponentsFileClicked(bool checked);
        void onNavigateToCustomTypesFileClicked(bool checked);
        void onNavigateToExportTemplateFileClicked(bool checked);
        void onNavigateToFieldDefinitionsFileClicked(bool checked);
        void onNavigateToRecordsFileClicked(bool checked);

        void onRemoveComponentsFileClicked(bool checked);
        void onRemoveCustomTypesFileClicked(bool checked);
        void onRemoveExportTemplateFileClicked(bool checked);
        void onRemoveFieldDefinitionsFileClicked(bool checked);
        void onRemoveRecordsFileClicked(bool checked);
};

#endif // PROJECTOVERVIEWWINDOW_H
