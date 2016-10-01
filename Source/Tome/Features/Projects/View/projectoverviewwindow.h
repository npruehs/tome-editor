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

        QListWidgetItem* getSelectedFieldDefinitionSet();
        QListWidgetItem* getSelectedRecordSet();

        void navigateToSelectedFile(QListWidgetItem* item);

        void updateFieldDefinitionData();
        void updateRecordData();

    private slots:
        void onAddExistingFieldDefinitionsFileClicked(bool checked);
        void onAddExistingRecordsFileClicked(bool checked);

        void onAddNewFieldDefinitionsFileClicked(bool checked);
        void onAddNewRecordsFileClicked(bool checked);

        void onNavigateToFieldDefinitionsFileClicked(bool checked);
        void onNavigateToRecordsFileClicked(bool checked);

        void onRemoveFieldDefinitionsFileClicked(bool checked);
        void onRemoveRecordsFileClicked(bool checked);
};

#endif // PROJECTOVERVIEWWINDOW_H
