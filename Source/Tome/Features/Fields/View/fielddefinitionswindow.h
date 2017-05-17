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
    class ProjectController;
    class RecordsController;
    class TypesController;
    class UndoController;
}

/**
 * @brief Window for adding and removing field definitions.
 */
class FieldDefinitionsWindow : public QMainWindow
{
        Q_OBJECT

    public:
    /**
         * @brief Constructs a new window for adding and removing field definitions.
         * @param fieldDefinitionsController Controller for adding, updating and removing field definitions.
         * @param componentsController Controller for adding and removing components.
         * @param facetsController Controller for validating type facets.
         * @param findUsagesController Controller for finding usages of fields, records and custom types.
         * @param projectController Controller for creating, loading and saving projects.
         * @param recordsController Controller for adding, updating and removing records.
         * @param typesController Controller for adding, updating and removing custom types.
         * @param undoController Controller for performing undo-able commands.
         * @param parent Optional owner widget.
         */
        explicit FieldDefinitionsWindow(
                Tome::FieldDefinitionsController& fieldDefinitionsController,
                Tome::ComponentsController& componentsController,
                Tome::FacetsController& facetsController,
                Tome::FindUsagesController& findUsagesController,
                Tome::ProjectController& projectController,
                Tome::RecordsController& recordsController,
                Tome::TypesController& typesController,
                Tome::UndoController& undoController,
                QWidget* parent = 0);
        ~FieldDefinitionsWindow();

    signals:
        /**
         * @brief A field definition has changed.
         * @param fieldId Id of the field that has changed.
         */
        void fieldChanged(const QString fieldId);

    protected:
        /**
         * @brief Updates the view, if necessary, and shows the window.
         * @param event Event for showing the window.
         */
        void showEvent(QShowEvent* event);

    private slots:
        void on_actionNew_Field_triggered();
        void on_actionEdit_Field_triggered();
        void on_actionDelete_Field_triggered();

        void on_actionFind_Usages_triggered();

        void on_tableWidget_doubleClicked(const QModelIndex &index);

        void onFieldDefinitionAdded(const Tome::FieldDefinition& fieldDefinition);
        void onFieldDefinitionRemoved(const Tome::FieldDefinition& fieldDefinition);
        void onFieldDefinitionUpdated(const Tome::FieldDefinition& oldFieldDefinition, const Tome::FieldDefinition& newFieldDefinition);

        void tableWidgetSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected);

    private:
        Ui::FieldDefinitionsWindow *ui;

        Tome::FieldDefinitionsController& fieldDefinitionsController;
        Tome::ComponentsController& componentsController;
        Tome::FacetsController& facetsController;
        Tome::FindUsagesController& findUsagesController;
        Tome::ProjectController& projectController;
        Tome::RecordsController& recordsController;
        Tome::TypesController& typesController;
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
