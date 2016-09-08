#ifndef DUPLICATERECORDWINDOW_H
#define DUPLICATERECORDWINDOW_H

#include <QDialog>

namespace Ui {
    class DuplicateRecordWindow;
}

class DuplicateRecordWindow : public QDialog
{
        Q_OBJECT

    public:
        explicit DuplicateRecordWindow(QWidget *parent = 0);
        ~DuplicateRecordWindow();

        QString getRecordId() const;

        void setDisallowedRecordIds(const QStringList disallowedRecordIds);
        void setRecordId(const QString& id);

    public slots:
        void accept();

    protected:
        void showEvent(QShowEvent* event);

    private:
        Ui::DuplicateRecordWindow *ui;

        QStringList disallowedRecordIds;

        bool validate();
};

#endif // DUPLICATERECORDWINDOW_H
