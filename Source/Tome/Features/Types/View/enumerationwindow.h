#ifndef ENUMERATIONWINDOW_H
#define ENUMERATIONWINDOW_H

#include <QDialog>

#include "../Model/customtype.h"
#include "enumerationmemberwindow.h"

namespace Ui {
    class EnumerationWindow;
}

class EnumerationWindow : public QDialog
{
        Q_OBJECT

    public:
        explicit EnumerationWindow(QWidget *parent = 0);
        ~EnumerationWindow();

        QString getEnumerationName() const;
        QStringList getEnumerationMembers() const;

        void setEnumerationName(const QString& typeName);
        void setEnumerationMembers(const QStringList enumeration);

    private slots:
        void on_actionNew_Member_triggered();
        void on_actionDelete_Member_triggered();

    private:
        Ui::EnumerationWindow *ui;

        EnumerationMemberWindow* enumerationMemberWindow;

        QStringList enumeration;
};

#endif // ENUMERATIONWINDOW_H
