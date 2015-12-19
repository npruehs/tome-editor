#ifndef FIELDDEFINITIONSWINDOW_H
#define FIELDDEFINITIONSWINDOW_H

#include <QMainWindow>
#include <QSharedPointer>

#include "../Projects/project.h"

#include "fielddefinitionwindow.h"
#include "fielddefinitionstablemodel.h"


namespace Ui {
    class FieldDefinitionsWindow;
}

class FieldDefinitionsWindow : public QMainWindow
{
        Q_OBJECT

    public:
        explicit FieldDefinitionsWindow(QSharedPointer<Tome::Project> project, QWidget *parent = 0);
        ~FieldDefinitionsWindow();

    private slots:
        void on_actionNew_Field_triggered();
        void on_actionEdit_Field_triggered();

        void on_tableView_doubleClicked(const QModelIndex &index);

    private:
        Ui::FieldDefinitionsWindow *ui;

        FieldDefinitionWindow* fieldDefinitionWindow;

        QSharedPointer<Tome::Project> project;
        QSharedPointer<Tome::FieldDefinitionsTableModel> viewModel;
};

#endif // FIELDDEFINITIONSWINDOW_H
