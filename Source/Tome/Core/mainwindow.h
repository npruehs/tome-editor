#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>

#include "../Features/Tasks/Model/messagelist.h"

class ComponentsWindow;
class FieldDefinitionsWindow;
class FieldValueWindow;
class AboutWindow;
class NewProjectWindow;
class RecordWindow;
class CustomTypesWindow;

namespace Ui {
    class MainWindow;
}

namespace Tome
{
    class Controller;
    class Project;
    class RecordTreeWidget;
    class RecordTreeWidgetItem;
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

        void on_actionRun_Integrity_Checks_triggered();

        void on_actionAbout_triggered();
        void on_actionManual_triggered();
        void on_actionReport_a_Bug_triggered();
        void on_actionReleases_triggered();

        void on_actionError_List_triggered();

        void on_toolButtonErrors_toggled(bool checked);
        void on_toolButtonWarnings_toggled(bool checked);
        void on_toolButtonMessages_toggled(bool checked);

        void exportRecords(QAction* exportAction);
        void openRecentProject(QAction* recentProjectAction);
        void revertFieldValue();
        void tableWidgetDoubleClicked(const QModelIndex &index);
        void treeWidgetDoubleClicked(const QModelIndex &index);
        void treeWidgetRecordReparented(const QString& recordId, const QString& newParentId);
        void treeWidgetSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected);

    private:
        Ui::MainWindow *ui;

        Tome::RecordTreeWidget* treeWidget;
        QTableWidget* tableWidget;

        Tome::Controller* controller;

        AboutWindow *aboutWindow;
        ComponentsWindow* componentsWindow;
        CustomTypesWindow* customTypesWindow;
        FieldDefinitionsWindow* fieldDefinitionsWindow;
        FieldValueWindow* fieldValueWindow;
        NewProjectWindow *newProjectWindow;
        RecordWindow* recordWindow;

        Tome::MessageList messages;

        void addRecordField(const QString& fieldId);
        QString getSelectedRecordId() const;
        Tome::RecordTreeWidgetItem* getSelectedRecordItem() const;
        void openProject(QString path);
        void removeRecordField(const QString& fieldId);
        void onProjectChanged();
        void refreshErrorList();
        void refreshRecordTree();
        void refreshRecordTable();
        void resetFields();
        void resetRecords();
        void showWindow(QWidget* widget);
        void updateMenus();
        void updateRecentProjects();
        void updateRecord(const QString& id, const QString& displayName);
        void updateRecordRow(const int i);
        void updateWindowTitle();
};

#endif // MAINWINDOW_H
