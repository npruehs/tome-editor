#ifndef CUSTOMTYPEMEMBERWINDOW_H
#define CUSTOMTYPEMEMBERWINDOW_H

#include <QDialog>

namespace Ui {
    class CustomTypeMemberWindow;
}

class CustomTypeMemberWindow : public QDialog
{
        Q_OBJECT

    public:
        explicit CustomTypeMemberWindow(QWidget *parent = 0);
        ~CustomTypeMemberWindow();

        QString getText() const;

    public slots:
        void accept();

    private:
        Ui::CustomTypeMemberWindow *ui;

        bool validate();
};

#endif // CUSTOMTYPEMEMBERWINDOW_H
