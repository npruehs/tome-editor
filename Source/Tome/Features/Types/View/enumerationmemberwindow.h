#ifndef ENUMERATIONMEMBERWINDOW_H
#define ENUMERATIONMEMBERWINDOW_H

#include <QDialog>

namespace Ui {
    class EnumerationMemberWindow;
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

    protected:
        virtual void showEvent(QShowEvent* event);

    private:
        Ui::EnumerationMemberWindow *ui;

        bool validate();
};

#endif // ENUMERATIONMEMBERWINDOW_H
