#ifndef CUSTOMTYPEWINDOW_H
#define CUSTOMTYPEWINDOW_H

#include <QDialog>

#include "customtype.h"
#include "enumerationmemberwindow.h"

namespace Ui {
    class CustomTypeWindow;
}

class EnumerationWindow : public QDialog
{
        Q_OBJECT

    public:
        explicit EnumerationWindow(QWidget *parent = 0);
        ~EnumerationWindow();

        QString getCustomTypeName() const;
        QStringList getCustomTypeEnumeration() const;

        void setCustomTypeName(const QString& typeName);
        void setCustomTypeEnumeration(const QStringList& enumeration);

    private slots:
        void on_actionNew_Member_triggered();
        void on_actionDelete_Member_triggered();

    private:
        Ui::CustomTypeWindow *ui;

        EnumerationMemberWindow* enumerationMemberWindow;
};

#endif // CUSTOMTYPEWINDOW_H
