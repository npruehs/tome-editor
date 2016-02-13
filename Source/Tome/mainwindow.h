#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSharedPointer>

#include "controller.h"
#include "Components/View/componentswindow.h"
#include "Fields/View/fielddefinitionswindow.h"
#include "Fields/View/fieldvaluewindow.h"
#include "Help/View/aboutwindow.h"
#include "Projects/View/newprojectwindow.h"
#include "Projects/Model/project.h"
#include "Records/View/recordwindow.h"
#include "Types/View/customtypeswindow.h"


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

    private slots:
        void on_actionExit_triggered();

        void on_actionField_Definions_triggered();
        void on_actionManage_Components_triggered();
        void on_actionManage_Custom_Types_triggered();

        void on_actionNew_Project_triggered();
        void on_actionOpen_Project_triggered();
        void on_actionSave_Project_triggered();

        void on_actionNew_Record_triggered();
        void on_actionEdit_Record_triggered();
        void on_actionRemove_Record_triggered();

        void on_actionAbout_triggered();
        void on_actionManual_triggered();
        void on_actionReport_a_Bug_triggered();
        void on_actionReleases_triggered();

        void on_treeWidget_doubleClicked(const QModelIndex &index);
        void on_tableWidget_doubleClicked(const QModelIndex &index);

        void exportRecords(QAction* exportAction);
        void openRecentProject(QAction* recentProjectAction);
        void treeViewSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected);

    private:
        Ui::MainWindow *ui;

        QSharedPointer<Tome::Controller> controller;

        AboutWindow *aboutWindow;
        ComponentsWindow* componentsWindow;
        CustomTypesWindow* customTypesWindow;
        FieldDefinitionsWindow* fieldDefinitionsWindow;
        FieldValueWindow* fieldValueWindow;
        NewProjectWindow *newProjectWindow;
        RecordWindow* recordWindow;

        void addRecordField(const QString& fieldId);
        QString getSelectedRecordDisplayName() const;
        void openProject(QString path);
        void removeRecordField(const QString& fieldId);
        void onProjectChanged();
        void showWindow(QWidget* widget);
        void updateMenus();
        void updateRecentProjects();
        void updateRecord(const QString& id, const QString& displayName);
        void updateRecordRow(const int i);
        void updateWindowTitle();
};

#endif // MAINWINDOW_H
