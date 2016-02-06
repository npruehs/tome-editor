#ifndef FIELDDEFINITIONSWINDOW_H
#define FIELDDEFINITIONSWINDOW_H

#include <QItemSelection>
#include <QMainWindow>
#include <QSharedPointer>

#include "../Projects/project.h"

#include "fielddefinitionwindow.h"


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
        void on_actionDelete_Field_triggered();

        void on_tableWidget_doubleClicked(const QModelIndex &index);

        void tableWidgetSelectionChanged(const QItemSelection & selected, const QItemSelection & deselected);


    private:
        Ui::FieldDefinitionsWindow *ui;

        FieldDefinitionWindow* fieldDefinitionWindow;

        QSharedPointer<Tome::Project> project;

        int getSelectedFieldIndex() const;
        void updateMenus();

        void updateFieldDefinition(const int index, const QString& id, const QString& displayName, const QString& fieldType, const QString& defaultValue, const QString& description, const QString& component);
        void updateRow(const int index);
};

#endif // FIELDDEFINITIONSWINDOW_H
