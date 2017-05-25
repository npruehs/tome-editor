#ifndef MAPITEMWINDOW_H
#define MAPITEMWINDOW_H

#include <QDialog>

namespace Ui {
    class MapItemWindow;
}

namespace Tome
{
    class FieldValueWidget;
    class FacetsController;
    class ProjectController;
    class RecordsController;
    class TypesController;
}

/**
 * @brief Window for editing the a single key-value pair of the map.
 */
class MapItemWindow : public QDialog
{
        Q_OBJECT

    public:
        /**
         * @brief Constructs a new window for editing the a single key-value pair of the map.
         * @param facetsController Controller for validating type facets.
         * @param projectController Controller for creating, loading and saving projects.
         * @param recordsController Controller for adding, updating and removing records.
         * @param typesController Controller for adding, updating and removing custom types.
         * @param parent Optional owner widget.
         */
        explicit MapItemWindow(Tome::FacetsController& facetsController,
                               Tome::ProjectController& projectController,
                               Tome::RecordsController& recordsController,
                               Tome::TypesController& typesController,
                               QWidget *parent = 0);
        ~MapItemWindow();

        /**
         * @brief Gets the current key of the key-value pair.
         * @return Current key of the key-value pair.
         */
        QVariant getKey() const;

        /**
         * @brief Gets the current value of the key-value pair.
         * @return Current value of the key-value pair.
         */
        QVariant getValue() const;

        /**
         * @brief Sets the current key of the key-value pair.
         * @param key Current key of the key-value pair.
         */
        void setKey(const QVariant& key);

        /**
         * @brief Sets the type of the keys of the map.
         * @param keyType Type of the keys of the map.
         */
        void setKeyType(const QString& keyType) const;

        /**
         * @brief Sets the current value of the key-value pair.
         * @param value Current value of the key-value pair.
         */
        void setValue(const QVariant& value);

        /**
         * @brief Sets the type of the values of the map.
         * @param valueType Type of the values of the map.
         */
        void setValueType(const QString& valueType) const;

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
        virtual void showEvent(QShowEvent* event) Q_DECL_OVERRIDE;

    private:
        Ui::MapItemWindow *ui;
        Tome::FieldValueWidget* keyWidget;
        Tome::FieldValueWidget* valueWidget;

        Tome::FacetsController& facetsController;
        Tome::ProjectController& projectController;
        Tome::RecordsController& recordsController;
        Tome::TypesController& typesController;

        bool validate();
};

#endif // MAPITEMWINDOW_H
