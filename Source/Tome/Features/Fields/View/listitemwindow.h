#ifndef LISTITEMWINDOW_H
#define LISTITEMWINDOW_H

#include <QDialog>
#include <QString>
#include <QStringList>

#include "../../Records/Controller/recordscontroller.h"
#include "../../Types/Controller/typescontroller.h"
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
        explicit ListItemWindow(Tome::RecordsController& recordsController, Tome::TypesController& typesController, QWidget *parent = 0);
        ~ListItemWindow();

        QVariant getValue() const;
        void setValue(const QVariant& value);

        void setFieldType(const QString& fieldType) const;

    private:
        Ui::ListItemWindow *ui;
        Tome::FieldValueWidget* fieldValueWidget;

        Tome::RecordsController& recordsController;
        Tome::TypesController& typesController;
};

#endif // LISTITEMWINDOW_H
