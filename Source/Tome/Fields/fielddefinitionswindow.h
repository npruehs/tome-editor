#ifndef FIELDDEFINITIONWINDOW_H
#define FIELDDEFINITIONWINDOW_H

#include <QDialog>
#include <QSharedPointer>

#include "../Projects/project.h"

#include "fielddefinitionstablemodel.h"

namespace Ui {
    class FieldDefinitionsWindow;
}

class FieldDefinitionsWindow : public QDialog
{
        Q_OBJECT

    public:
        FieldDefinitionsWindow(QSharedPointer<Tome::Project> project, QWidget *parent = 0);
        ~FieldDefinitionsWindow();

    private:
        Ui::FieldDefinitionsWindow *ui;

        QSharedPointer<Tome::Project> project;
        QSharedPointer<Tome::FieldDefinitionsTableModel> viewModel;
};

#endif // FIELDDEFINITIONWINDOW_H
