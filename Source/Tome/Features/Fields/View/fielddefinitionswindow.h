#ifndef FIELDDEFINITIONSWINDOW_H
#define FIELDDEFINITIONSWINDOW_H

#include <QItemSelection>
#include <QMainWindow>

#include "../../Components/Model/component.h"

class FieldDefinitionWindow;

namespace Ui {
    class FieldDefinitionsWindow;
}

namespace Tome
{
    class ComponentsController;
    class FieldDefinitionsController;
    class RecordsController;
    class TypesController;
}

class FieldDefinitionsWindow : public QMainWindow
{
        Q_OBJECT

    public:
        explicit FieldDefinitionsWindow(
                Tome::FieldDefinitionsController& fieldDefinitionsController,
                Tome::ComponentsController& componentsController,
                Tome::RecordsController& recordsController,
                Tome::TypesController& typesController,
                QWidget *parent = 0);
        ~FieldDefinitionsWindow();

    private slots:
        void on_actionNew_Field_triggered();
        void on_actionEdit_Field_triggered();
        void on_actionDelete_Field_triggered();

        void on_tableWidget_doubleClicked(const QModelIndex &index);

        void tableWidgetSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected);


    private:
        Ui::FieldDefinitionsWindow *ui;

        Tome::FieldDefinitionsController& fieldDefinitionsController;
        Tome::ComponentsController& componentsController;
        Tome::RecordsController& recordsController;
        Tome::TypesController& typesController;

        FieldDefinitionWindow* fieldDefinitionWindow;

        int getSelectedFieldIndex() const;
        void updateMenus();

        void updateFieldDefinition(const QString& oldId, const QString& newId, const QString& displayName, const QString& fieldType, const QVariant& defaultValue, const QString& description, const Tome::Component& component);
        void updateRow(const int index);
};

#endif // FIELDDEFINITIONSWINDOW_H
