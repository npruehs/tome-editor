#ifndef FINDRECORDWINDOW_H
#define FINDRECORDWINDOW_H

#include <QDialog>

namespace Ui {
    class FindRecordWindow;
}

class FindRecordWindow : public QDialog
{
        Q_OBJECT

    public:
        explicit FindRecordWindow(QWidget *parent = 0);
        ~FindRecordWindow();

        QString getSearchPattern() const;

    private:
        Ui::FindRecordWindow *ui;
};

#endif // FINDRECORDWINDOW_H
