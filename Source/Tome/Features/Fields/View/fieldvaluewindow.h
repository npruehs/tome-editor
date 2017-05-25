#ifndef FIELDVALUEWINDOW_H
#define FIELDVALUEWINDOW_H

#include <QDialog>
#include <QVariantMap>

namespace Ui {
    class FieldValueWindow;
}

namespace Tome
{
    class CustomType;
    class Facet;
    class FieldValueWidget;
    class FacetsController;
    class ProjectController;
    class RecordsController;
    class TypesController;
}

/**
 * @brief Window for editing the value of a record field.
 */
class FieldValueWindow : public QDialog
{
        Q_OBJECT

    public:
        /**
         * @brief Constructs a new window for editing the value of a record field.
         * @param facetsController Controller for validating type facets.
         * @param projectController Controller for creating, loading and saving projects.
         * @param recordsController Controller for adding, updating and removing records.
         * @param typesController Controller for adding, updating and removing custom types.
         * @param parent Optional owner widget.
         */
        explicit FieldValueWindow(Tome::FacetsController& facetsController,
                                  Tome::ProjectController& projectController,
                                  Tome::RecordsController& recordsController,
                                  Tome::TypesController& typesController,
                                  QWidget* parent = 0);
        ~FieldValueWindow();

        /**
         * @brief Gets the current value of the field.
         * @return Current value of the field.
         */
        QVariant getFieldValue() const;

        /**
         * @brief Sets the number of field values that are simultaneously edited.
         * @param fieldCount Number of field values that are simultaneously edited.
         */
        void setFieldCount(const int fieldCount);

        /**
         * @brief Sets the textual description of the field to be shown.
         * @param description Textual description of the field.
         */
        void setFieldDescription(const QString& description);

        /**
         * @brief Sets the display name of the field to be shown.
         * @param displayName Display name of the field.
         */
        void setFieldDisplayName(const QString& displayName);

        /**
         * @brief Sets the current value of the field.
         * @param fieldValue Current value of the field.
         */
        void setFieldValue(const QVariant& fieldValue);

        /**
         * @brief Sets the type of the edited field.
         * @param fieldType Type of the edited field.
         */
        void setFieldType(const QString& fieldType) const;

    public slots:
        /**
         * @brief Validates all data and closes this window if successful.
         */
        void accept();

    signals:
        /**
         * @brief Field should be reverted to its original value.
         */
        void revert();

    protected:
        /**
         * @brief Sets up this window, e.g. by setting the focus on the first widget.
         * @param event Event for showing this window.
         */
        virtual void showEvent(QShowEvent* event) Q_DECL_OVERRIDE;

    private slots:
        void on_toolButtonRevert_clicked();

    private:
        Ui::FieldValueWindow *ui;
        Tome::FieldValueWidget* fieldValueWidget;

        Tome::FacetsController& facetsController;
        Tome::ProjectController& projectController;
        Tome::RecordsController& recordsController;
        Tome::TypesController& typesController;
};

#endif // FIELDVALUEWINDOW_H
