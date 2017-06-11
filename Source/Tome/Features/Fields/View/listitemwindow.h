#ifndef LISTITEMWINDOW_H
#define LISTITEMWINDOW_H

#include <QDialog>

namespace Ui {
    class ListItemWindow;
}

namespace Tome
{
    class CustomType;
    class FieldValueWidget;
    class FacetsController;
    class ProjectController;
    class RecordsController;
    class TypesController;
}

/**
 * @brief Window for editing the value of a single list item.
 */
class ListItemWindow : public QDialog
{
        Q_OBJECT

    public:
        /**
         * @brief Constructs a new window for editing the value of a single list item.
         * @param facetsController Controller for validating type facets.
         * @param projectController Controller for creating, loading and saving projects.
         * @param recordsController Controller for adding, updating and removing records.
         * @param typesController Controller for adding, updating and removing custom types.
         * @param parent Optional owner widget.
         */
        explicit ListItemWindow(Tome::FacetsController& facetsController,
                                Tome::ProjectController& projectController,
                                Tome::RecordsController& recordsController,
                                Tome::TypesController& typesController,
                                QWidget *parent = 0);
        ~ListItemWindow();

        /**
         * @brief Gets the current value of the list item.
         * @return Current value of the list item.
         */
        QVariant getValue() const;

        /**
         * @brief Sets the current value of the list item.
         * @param value Current value of the list item.
         */
        void setValue(const QVariant& value);

        /**
         * @brief Sets the type of the list item.
         * @param fieldType Type of the list item.
         */
        void setFieldType(const QString& fieldType) const;

    public slots:
        /**
         * @brief Validates all data and closes this window if successful.
         */
        void accept() Q_DECL_OVERRIDE;

    protected:
        /**
         * @brief Sets up this window, e.g. by setting the focus on the first widget.
         * @param event Event for showing this window.
         */
        virtual void showEvent(QShowEvent* event) Q_DECL_OVERRIDE;

    private:
        Ui::ListItemWindow *ui;
        Tome::FieldValueWidget* fieldValueWidget;

        Tome::FacetsController& facetsController;
        Tome::ProjectController& projectController;
        Tome::RecordsController& recordsController;
        Tome::TypesController& typesController;

        bool validate();
};

#endif // LISTITEMWINDOW_H
