#ifndef CUSTOMTYPEWINDOW_H
#define CUSTOMTYPEWINDOW_H

#include <QDialog>

#include "customtype.h"
#include "customtypememberwindow.h"

namespace Ui {
    class CustomTypeWindow;
}

class CustomTypeWindow : public QDialog
{
        Q_OBJECT

    public:
        explicit CustomTypeWindow(QWidget *parent = 0);
        ~CustomTypeWindow();

        QString getCustomTypeName() const;
        QStringList getCustomTypeEnumeration() const;

        void setCustomTypeName(const QString& typeName);
        void setCustomTypeEnumeration(const QStringList& enumeration);

    private slots:
        void on_actionNew_Member_triggered();
        void on_actionDelete_Member_triggered();

    private:
        Ui::CustomTypeWindow *ui;

        CustomTypeMemberWindow* customTypeMemberWindow;
};

#endif // CUSTOMTYPEWINDOW_H
