#ifndef LISTITEMWINDOW_H
#define LISTITEMWINDOW_H

#include <QDialog>
#include <QString>
#include <QStringList>

#include "../../Types/Model/customtype.h"


namespace Ui {
    class ListItemWindow;
}

namespace Tome {
    class FieldValueWidget;
}

class ListItemWindow : public QDialog
{
        Q_OBJECT

    public:
        explicit ListItemWindow(QWidget *parent = 0);
        ~ListItemWindow();

        QString getValue() const;
        void setValue(const QString& value);

        void setCustomFieldType(const Tome::CustomType& fieldType);
        void setFieldType(const QString& fieldType) const;
        void setEnumeration(const QStringList& recordNames);

    private:
        Ui::ListItemWindow *ui;
        Tome::FieldValueWidget* fieldValueWidget;
};

#endif // LISTITEMWINDOW_H
