#ifndef MAPWINDOW_H
#define MAPWINDOW_H

#include <QDialog>

namespace Ui {
    class MapWindow;
}

namespace Tome
{
    class TypesController;
}

/**
 * @brief Window for adding a new map type or editing an existing one.
 */
class MapWindow : public QDialog
{
        Q_OBJECT

    public:
        /**
         * @brief Constructs a new window for adding a new map type or editing an existing one.
         * @param typesController Controller for adding, updating and removing custom types.
         * @param parent Optional owner widget.
         */
        explicit MapWindow(Tome::TypesController& typesController, QWidget *parent = 0);
        ~MapWindow();

        /**
         * @brief Gets the name of the map type.
         * @return Name of the map type.
         */
        QString getMapName() const;

        /**
         * @brief Gets the type of the keys of the map.
         * @return Type of the keys of the map.
         */
        QString getMapKeyType() const;

        /**
         * @brief Gets the type of the values of the map.
         * @return Type of the values of the map.
         */
        QString getMapValueType() const;

        /**
         * @brief Gets the name of the set the map type should belong to.
         * @return Name of the set the map type should belong to.
         */
        QString getTypeSetName() const;

        /**
         * @brief Fills all combo boxes of this window with current data.
         */
        void init();

        /**
         * @brief Sets the name of the map type.
         * @param mapName Name of the map type.
         */
        void setMapName(const QString& mapName);

        /**
         * @brief Sets the type of the keys of the map.
         * @param keyType Type of the keys of the map.
         */
        void setMapKeyType(const QString& keyType);

        /**
         * @brief Sets the type of the values of the map
         * @param valueType Type of the values of the map.
         */
        void setMapValueType(const QString& valueType);

        /**
         * @brief Sets the name of the set the map type should belong to.
         * @param typeSetName Name of the set the map type should belong to.
         */
        void setTypeSetName(const QString& typeSetName);

        /**
         * @brief Sets the list of names of available custom type sets.
         * @param typeSetNames List of names of available custom type sets.
         */
        void setTypeSetNames(const QStringList& typeSetNames);

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
        Ui::MapWindow *ui;
        Tome::TypesController& typesController;

        bool validate();
};

#endif // MAPWINDOW_H
