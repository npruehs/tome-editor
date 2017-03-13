#ifndef NEWPROJECTWINDOW_H
#define NEWPROJECTWINDOW_H

#include <QDialog>
#include <QString>

#include "../Model/recordidtype.h"

namespace Ui {
    class NewProjectWindow;
}

class NewProjectWindow : public QDialog
{
        Q_OBJECT

    public:
        explicit NewProjectWindow(QWidget *parent = 0);
        ~NewProjectWindow();

        QString getProjectName() const;
        QString getProjectPath() const;
        Tome::RecordIdType::RecordIdType getProjectRecordIdType() const;

    private slots:
        void on_comboBoxRecordIdType_currentTextChanged(const QString &text);
        void on_pushButtonBrowse_clicked();

    private:
        Ui::NewProjectWindow *ui;
};

#endif // NEWPROJECTWINDOW_H
