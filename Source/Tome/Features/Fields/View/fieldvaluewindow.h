#ifndef FIELDVALUEWINDOW_H
#define FIELDVALUEWINDOW_H

#include <QDialog>

#include "fieldvaluewidget.h"
#include "../../Records/Controller/recordscontroller.h"
#include "../../Types/Controller/typescontroller.h"
#include "../../Types/Model/customtype.h"


namespace Ui {
    class FieldValueWindow;
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

    protected:
        virtual void showEvent(QShowEvent* event);

    private:
        Ui::FieldValueWindow *ui;
        Tome::FieldValueWidget* fieldValueWidget;

        Tome::RecordsController& recordsController;
        Tome::TypesController& typesController;
};

#endif // FIELDVALUEWINDOW_H
