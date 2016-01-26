#ifndef CUSTOMTYPEMEMBERWINDOW_H
#define CUSTOMTYPEMEMBERWINDOW_H

#include <QDialog>

namespace Ui {
    class CustomTypeMemberWindow;
}

class EnumerationMemberWindow : public QDialog
{
        Q_OBJECT

    public:
        explicit EnumerationMemberWindow(QWidget *parent = 0);
        ~EnumerationMemberWindow();

        QString getText() const;

    public slots:
        void accept();

    private:
        Ui::CustomTypeMemberWindow *ui;

        bool validate();
};

#endif // CUSTOMTYPEMEMBERWINDOW_H
