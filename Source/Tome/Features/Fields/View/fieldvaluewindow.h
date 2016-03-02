#ifndef FIELDVALUEWINDOW_H
#define FIELDVALUEWINDOW_H

#include <QDialog>

namespace Ui {
    class FieldValueWindow;
}

namespace Tome
{
    class CustomType;
    class FieldValueWidget;
    class RecordsController;
    class TypesController;
}

class FieldValueWindow : public QDialog
{
        Q_OBJECT

    public:
        explicit FieldValueWindow(Tome::RecordsController& recordsController, Tome::TypesController& typesController, QWidget *parent = 0);
        ~FieldValueWindow();

        QVariant getFieldValue() const;

        void setFieldDescription(const QString& description);
        void setFieldDisplayName(const QString& displayName);
        void setFieldValue(const QVariant& fieldValue);
        void setFieldType(const QString& fieldType) const;

    signals:
        revert();

    protected:
        virtual void showEvent(QShowEvent* event);

    private slots:
        void on_toolButtonRevert_clicked();

    private:
        Ui::FieldValueWindow *ui;
        Tome::FieldValueWidget* fieldValueWidget;

        Tome::RecordsController& recordsController;
        Tome::TypesController& typesController;
};

#endif // FIELDVALUEWINDOW_H
