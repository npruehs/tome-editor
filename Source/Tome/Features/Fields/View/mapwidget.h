#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QHBoxLayout>
#include <QTableWidget>
#include <QString>
#include <QToolButton>
#include <QVBoxLayout>
#include <QWidget>

class MapItemWindow;

namespace Tome
{
    class FacetsController;
    class ProjectController;
    class RecordsController;
    class TypesController;

    /**
     * @brief Allows adding, editing, and removing map items.
     */
    class MapWidget : public QWidget
    {
            Q_OBJECT

        public:
            /**
             * @brief Constructs a new widget for adding, editing, and removing map items.
             * @param facetsController Controller for validating type facets.
             * @param projectController Controller for creating, loading and saving projects.
             * @param recordsController Controller for adding, updating and removing records.
             * @param typesController Controller for adding, updating and removing custom types.
             * @param parent Optional owner widget.
             */
            explicit MapWidget(FacetsController& facetsController,
                               ProjectController& projectController,
                               RecordsController& recordsController,
                               TypesController& typesController,
                               QWidget *parent = 0);
            ~MapWidget();

            /**
             * @brief Gets the type of the keys of the map.
             * @return Type of the keys of the map.
             */
            QString getKeyType() const;

            /**
             * @brief Gets the type of the values of the map.
             * @return Type of the values of the map.
             */
            QString getValueType() const;

            /**
             * @brief Gets all current map items.
             * @return All current map items.
             */
            QVariantMap getMap() const;

            /**
             * @brief Gets the type of the keys of the map.
             * @param keyType Type of the keys of the map.
             */
            void setKeyType(const QString& keyType);

            /**
             * @brief Gets the type of the values of the map.
             * @param valueType Type of the values of the map.
             */
            void setValueType(const QString& valueType);

            /**
             * @brief Sets all current map items.
             * @param map All current map items.
             */
            void setMap(const QVariantMap& map);

        private slots:
            void addItem();
            void editItem(QTableWidgetItem* item);
            void removeItem();

            void clearTable();

        private:
            QString keyType;
            QString valueType;

            QVariantMap map;

            MapItemWindow* mapItemWindow;

            QHBoxLayout* layout;
            QVBoxLayout* buttonLayout;
            QTableWidget* tableWidget;

            FacetsController& facetsController;
            ProjectController& projectController;
            RecordsController& recordsController;
            TypesController& typesController;

            QTableWidgetItem* createTableWidgetItem(QVariant value, bool isReference);
    };
}

#endif // MAPWIDGET_H
