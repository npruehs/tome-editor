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
    class FacetsController;
    class FieldDefinition;
    class FieldDefinitionsController;
    class FindUsagesController;
    class RecordsController;
    class TypesController;
    class UndoController;
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
                Tome::FindUsagesController& findUsagesController,
                Tome::FacetsController& facetsController,
                Tome::UndoController& undoController,
                QWidget *parent = 0);
        ~FieldDefinitionsWindow();

    signals:
        void fieldChanged();

    protected:
        void showEvent(QShowEvent * event);

    private slots:
        void on_actionNew_Field_triggered();
        void on_actionEdit_Field_triggered();
        void on_actionDelete_Field_triggered();

        void on_actionFind_Usages_triggered();

        void on_tableWidget_doubleClicked(const QModelIndex &index);

        void onFieldDefinitionAdded(const Tome::FieldDefinition& fieldDefinition);
        void onFieldDefinitionRemoved(const Tome::FieldDefinition& fieldDefinition);

        void tableWidgetSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected);

    private:
        Ui::FieldDefinitionsWindow *ui;

        Tome::FieldDefinitionsController& fieldDefinitionsController;
        Tome::ComponentsController& componentsController;
        Tome::RecordsController& recordsController;
        Tome::TypesController& typesController;
        Tome::FindUsagesController& findUsagesController;
        Tome::FacetsController& facetsController;
        Tome::UndoController& undoController;

        FieldDefinitionWindow* fieldDefinitionWindow;

        int getFieldRow(const QString& fieldId) const;
        QString getSelectedFieldId() const;

        void updateTable();
        void updateMenus();

        void updateFieldDefinition(
                const QString oldId,
                const QString newId,
                const QString& displayName,
                const QString& fieldType,
                const QVariant& defaultValue,
                const QString& description,
                const Tome::Component& component,
                const QString& fieldDefinitionSetName);
        void updateRow(const int index, const Tome::FieldDefinition& fieldDefinition, bool disableSorting);
};

#endif // FIELDDEFINITIONSWINDOW_H
