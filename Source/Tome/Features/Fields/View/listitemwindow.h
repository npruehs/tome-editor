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
    class RecordsController;
    class TypesController;
}

class ListItemWindow : public QDialog
{
        Q_OBJECT

    public:
        explicit ListItemWindow(Tome::FacetsController& facetsController, Tome::RecordsController& recordsController, Tome::TypesController& typesController, QWidget *parent = 0);
        ~ListItemWindow();

        QVariant getValue() const;
        void setValue(const QVariant& value);

        void setFieldType(const QString& fieldType) const;

    protected:
        virtual void showEvent(QShowEvent* event);

    private:
        Ui::ListItemWindow *ui;
        Tome::FieldValueWidget* fieldValueWidget;

        Tome::FacetsController& facetsController;
        Tome::RecordsController& recordsController;
        Tome::TypesController& typesController;
};

#endif // LISTITEMWINDOW_H
