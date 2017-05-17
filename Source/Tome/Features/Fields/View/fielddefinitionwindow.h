#ifndef FIELDDEFINITIONWINDOW_H
#define FIELDDEFINITIONWINDOW_H

#include <QDialog>
#include <QList>

#include "../../Components/Model/component.h"


namespace Ui {
    class FieldDefinitionWindow;
}

namespace Tome
{
    class ComponentsController;
    class FacetsController;
    class FieldDefinitionsController;
    class FieldValueWidget;
    class ProjectController;
    class RecordsController;
    class TypesController;
}

/**
 * @brief Window for adding a new field definition or editing an existing one.
 */
class FieldDefinitionWindow : public QDialog
{
        Q_OBJECT

    public:
        /**
         * @brief Constructs a new window for adding a new field definition or editing an existing one.
         * @param fieldDefinitionsController Controller for adding, updating and removing field definitions.
         * @param componentsController Controller for adding and removing components.
         * @param facetsController Controller for validating type facets.
         * @param projectController Controller for creating, loading and saving projects.
         * @param recordsController Controller for adding, updating and removing records.
         * @param typesController Controller for adding, updating and removing custom types.
         * @param parent Optional owner widget.
         */
        explicit FieldDefinitionWindow(Tome::FieldDefinitionsController& fieldDefinitionsController,
                Tome::ComponentsController& componentsController,
                Tome::FacetsController& facetsController,
                Tome::ProjectController& projectController,
                Tome::RecordsController& recordsController,
                Tome::TypesController& typesController,
                QWidget* parent = 0);
        ~FieldDefinitionWindow();

        /**
         * @brief Gets the component the field should belong to.
         * @return Component the field should belong to.
         */
        Tome::Component getFieldComponent() const;

        /**
         * @brief Gets the name of the set the field should belong to.
         * @return Name of the set the field should belong to.
         */
        QString getFieldDefinitionSetName() const;

        /**
         * @brief Gets the textual description of the field.
         * @return Textual description of the field.
         */
        QString getFieldDescription() const;

        /**
         * @brief Gets the display name of the field.
         * @return Display name of the field.
         */
        QString getFieldDisplayName() const;

        /**
         * @brief Gets the unique id of the field.
         * @return Unique id of the field.
         */
        QString getFieldId() const;

        /**
         * @brief Gets the default value of the field.
         * @return Default value of the field.
         */
        QVariant getDefaultValue() const;

        /**
         * @brief Gets the type of the values of the field.
         * @return Type of the values of the field.
         */
        QString getFieldType() const;

        /**
         * @brief Fills all comboboxes of this window with current data.
         */
        void init();

        /**
         * @brief Sets the component the field should belong to.
         * @param component Component the field should belong to.
         */
        void setFieldComponent(const QString& component) const;

        /**
         * @brief Sets the name of the set the field should belong to.
         * @param fieldDefinitionSetName Name of the set the field should belong to.
         */
        void setFieldDefinitionSetName(const QString& fieldDefinitionSetName);

        /**
         * @brief Sets the list of names of available field definition sets.
         * @param fieldDefinitionSetNames List of names of available field definition sets.
         */
        void setFieldDefinitionSetNames(const QStringList& fieldDefinitionSetNames);

        /**
         * @brief Sets the textual description of the field.
         * @param description Textual description of the field.
         */
        void setFieldDescription(const QString& description);

        /**
         * @brief Sets the display name of the field.
         * @param displayName Display name of the field.
         */
        void setFieldDisplayName(const QString& displayName);

        /**
         * @brief Sets the unique id of the field.
         * @param fieldId Unique id of the field.
         */
        void setFieldId(const QString& fieldId);

        /**
         * @brief Sets the default value of the field.
         * @param defaultValue Default value of the field.
         */
        void setDefaultValue(const QVariant& defaultValue);

        /**
         * @brief Sets the type of the values of the field.
         * @param fieldType Type of the values of the field.
         */
        void setFieldType(const QString& fieldType) const;

    public slots:
        /**
         * @brief Validates all data and closes this window if successful.
         */
        void accept();

    protected:
        /**
         * @brief Sets up this window, e.g. by setting the focus on the first widget.
         * @param event Event for showing this window.
         */
        void showEvent(QShowEvent* event);

    private slots:
        void on_comboBoxType_currentIndexChanged(const QString &fieldType);
        void on_lineEditDisplayName_textEdited(const QString &displayName);

    private:
        static const int DefaultFormRows;
        static const int ValueFormRow;

        Ui::FieldDefinitionWindow *ui;
        Tome::FieldDefinitionsController& fieldDefinitionsController;
        Tome::ComponentsController& componentsController;
        Tome::FacetsController& facetsController;
        Tome::ProjectController& projectController;
        Tome::RecordsController& recordsController;
        Tome::TypesController& typesController;

        Tome::FieldValueWidget* fieldValueWidget;

        bool validate();
};

#endif // FIELDDEFINITIONWINDOW_H
