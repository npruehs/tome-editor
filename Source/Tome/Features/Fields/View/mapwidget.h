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
    class RecordsController;
    class TypesController;

    class MapWidget : public QWidget
    {
            Q_OBJECT

        public:
            explicit MapWidget(RecordsController& recordsController, TypesController& typesController, QWidget *parent = 0);
            ~MapWidget();

            QString getKeyType() const;
            QString getValueType() const;
            QVariantMap getMap() const;

            void setKeyType(const QString& keyType);
            void setValueType(const QString& valueType);

            void setMap(const QVariantMap& map);

        private slots:
            void addItem();
            void editItem(QTableWidgetItem* item);
            void removeItem();

        private:
            QString keyType;
            QString valueType;

            QVariantMap map;

            MapItemWindow* mapItemWindow;

            QHBoxLayout* layout;
            QVBoxLayout* buttonLayout;
            QTableWidget* tableWidget;

            RecordsController& recordsController;
            TypesController& typesController;
    };
}

#endif // MAPWIDGET_H
