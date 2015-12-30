#ifndef FIELDVALUEWINDOW_H
#define FIELDVALUEWINDOW_H

#include <QDialog>

#include "../Fields/fieldvaluewidget.h"
#include "../Fields/fieldtype.h"


namespace Ui {
    class FieldValueWindow;
}

class FieldValueWindow : public QDialog
{
        Q_OBJECT

    public:
        explicit FieldValueWindow(QWidget *parent = 0);
        ~FieldValueWindow();

        QString getFieldValue() const;

        void setFieldDescription(const QString& description);
        void setFieldDisplayName(const QString& displayName);
        void setFieldValue(const QString& fieldValue);
        void setFieldType(const Tome::FieldType::FieldType& fieldType) const;
        void setRecordNames(const QStringList& recordNames);

    private:
        Ui::FieldValueWindow *ui;
        Tome::FieldValueWidget* fieldValueWidget;
};

#endif // FIELDVALUEWINDOW_H
