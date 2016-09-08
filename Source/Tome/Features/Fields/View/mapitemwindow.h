#ifndef MAPITEMWINDOW_H
#define MAPITEMWINDOW_H

#include <QDialog>

namespace Ui {
    class MapItemWindow;
}

namespace Tome
{
    class FieldValueWidget;
    class RecordsController;
    class TypesController;
}

class MapItemWindow : public QDialog
{
        Q_OBJECT

    public:
        explicit MapItemWindow(Tome::RecordsController& recordsController, Tome::TypesController& typesController, QWidget *parent = 0);
        ~MapItemWindow();

        QVariant getKey() const;
        QVariant getValue() const;
        void setKey(const QVariant& key);
        void setKeyType(const QString& keyType) const;
        void setValue(const QVariant& value);
        void setValueType(const QString& valueType) const;

    protected:
        virtual void showEvent(QShowEvent* event);

    private:
        Ui::MapItemWindow *ui;
        Tome::FieldValueWidget* keyWidget;
        Tome::FieldValueWidget* valueWidget;

        Tome::RecordsController& recordsController;
        Tome::TypesController& typesController;
};

#endif // MAPITEMWINDOW_H
