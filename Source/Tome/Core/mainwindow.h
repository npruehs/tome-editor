#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QCloseEvent>
#include <QMainWindow>
#include <QProgressDialog>
#include <QSharedPointer>
#include <QTableWidget>

#include "../Features/Search/Model/searchresultlist.h"
#include "../Features/Tasks/Model/messagelist.h"

class ComponentsWindow;
class FieldDefinitionsWindow;
class FieldValueWindow;
class AboutWindow;
class NewProjectWindow;
class RecordWindow;
class CustomTypesWindow;
class DuplicateRecordWindow;
class FindRecordWindow;
class ProjectOverviewWindow;
class UserSettingsWindow;

namespace Ui {
    class MainWindow;
}

namespace Tome
{
    class Controller;
    class ErrorListDockWidget;
    class OutputDockWidget;
    class Project;
    class RecordFieldsTableWidget;
    class RecordTreeWidget;
    class RecordTreeWidgetItem;
    class SearchResultsDockWidget;
}

class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        /**
         * @brief Constructs the main window and all other windows.
         * @param controller Controller owning the main window.
         * @param parent Optional owner widget.
         */
        explicit MainWindow(Tome::Controller* controller, QWidget *parent = 0);
        ~MainWindow();

    protected:
        /**
         * @brief Asks the user whether they want to save unsaved changes, and handles the close event accordingly.
         * @param event Event that wants to close the main window.
         */
        void closeEvent(QCloseEvent* event);

    private slots:
        void on_actionExit_triggered();

        void on_actionProject_Overview_triggered();

        void on_actionField_Definions_triggered();
        void on_actionManage_Components_triggered();
        void on_actionManage_Custom_Types_triggered();

        void on_actionNew_Project_triggered();
        void on_actionOpen_Project_triggered();
        void on_actionSave_Project_triggered();
        void on_actionReload_Project_triggered();

        void on_actionNew_Record_triggered();
        void on_actionAdd_Child_triggered();
        void on_actionEdit_Record_triggered();
        void on_actionDuplicate_Record_triggered();
        void on_actionRevert_Record_triggered();
        void on_actionRemove_Record_triggered();

        void on_actionFindRecord_triggered();
        void on_actionFind_Usages_triggered();

        void on_actionRun_Integrity_Checks_triggered();

        void on_actionUser_Settings_triggered();

        void on_actionAbout_triggered();
        void on_actionManual_triggered();
        void on_actionReport_a_Bug_triggered();
        void on_actionReleases_triggered();
        void on_actionRoadmap_triggered();

        void on_actionOutput_triggered();
        void on_actionError_List_triggered();
        void on_actionNavigate_Backward_triggered();
        void on_actionNavigate_Forward_triggered();

        void exportRecords(QAction* exportAction);
        void importRecords(QAction* importAction);
        void onExportTemplatesChanged();
        void onFieldChanged();
        void onImportError(const QString& error);
        void onImportFinished();
        void onImportStarted();
        void onImportTemplatesChanged();
        void onFileLinkActivated(const QString& filePath);
        void onProgressChanged(const QString title, const QString text, const int currentValue, const int maximumValue);
        void onProjectChanged(QSharedPointer<Tome::Project> project);
        void onRecordAdded(const QVariant& recordId, const QString& recordDisplayName, const QVariant& parentId);
        void onRecordFieldsChanged(const QVariant& recordId);
        void onRecordRemoved(const QVariant& recordId);
        void onRecordReparented(const QVariant& recordId, const QVariant& oldParentId, const QVariant& newParentId);
        void onRecordSetsChanged();
        void onRecordUpdated(const QVariant& oldId,
                             const QString& oldDisplayName,
                             const QString& oldEditorIconFieldId,
                             const QVariant& newId,
                             const QString& newDisplayName,
                             const QString& newEditorIconFieldId);
        void onRecordLinkActivated(const QString& recordId);
        void onUndoStackChanged(bool clean);
        void openRecentProject(QAction* recentProjectAction);
        void revertFieldValue();
        void searchResultChanged(const QString& title, const Tome::SearchResultList results);
        void tableWidgetDoubleClicked(const QModelIndex &index);
        void treeWidgetDoubleClicked(const QModelIndex &index);
        void treeWidgetRecordReparented(const QVariant& recordId, const QVariant& newParentId);
        void treeWidgetSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected);

    private:
        Ui::MainWindow *ui;

        Tome::RecordTreeWidget* recordTreeWidget;
        Tome::RecordFieldsTableWidget* recordFieldTableWidget;
        Tome::ErrorListDockWidget* errorListDockWidget;
        Tome::OutputDockWidget* outputDockWidget;
        Tome::SearchResultsDockWidget* searchResultsDockWidget;

        Tome::Controller* controller;

        AboutWindow *aboutWindow;
        ComponentsWindow* componentsWindow;
        CustomTypesWindow* customTypesWindow;
        FieldDefinitionsWindow* fieldDefinitionsWindow;
        FieldValueWindow* fieldValueWindow;
        NewProjectWindow *newProjectWindow;
        RecordWindow* recordWindow;
        DuplicateRecordWindow* duplicateRecordWindow;
        FindRecordWindow* findRecordWindow;
        ProjectOverviewWindow* projectOverviewWindow;
        UserSettingsWindow* userSettingsWindow;

        QProgressDialog* progressDialog;

        QList<QDockWidget*> dockWidgets;

        Tome::MessageList messages;

        bool refreshRecordTreeAfterReparent;

        QString getReadOnlyMessage(const QVariant& recordId);
        void openProject(QString path);
        void refreshErrorList();
        void refreshExportMenu();
        void refreshImportMenu();
        void refreshRecordTree();
        void refreshRecordTable();
        void showReadOnlyMessage(const QVariant& recordId);
        void showWindow(QWidget* widget);
        void updateMenus();
        void updateRecentProjects();
        void updateWindowTitle();
};

#endif // MAINWINDOW_H
