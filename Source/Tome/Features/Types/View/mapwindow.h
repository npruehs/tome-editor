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

class MapWindow : public QDialog
{
        Q_OBJECT

    public:
        explicit MapWindow(Tome::TypesController& typesController, QWidget *parent = 0);
        ~MapWindow();

        QString getMapName() const;
        QString getMapKeyType() const;
        QString getMapValueType() const;
        QString getTypeSetName() const;

        void init();

        void setMapName(const QString& mapName);
        void setMapKeyType(const QString& keyType);
        void setMapValueType(const QString& valueType);
        void setTypeSetName(const QString& typeSetName);
        void setTypeSetNames(const QStringList& typeSetNames);

    public slots:
        void accept();

    protected:
        virtual void showEvent(QShowEvent* event);

    private:
        Ui::MapWindow *ui;
        Tome::TypesController& typesController;

        bool validate();
};

#endif // MAPWINDOW_H
