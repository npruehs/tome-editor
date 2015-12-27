#ifndef RECORDWINDOW_H
#define RECORDWINDOW_H

#include <QDialog>

namespace Ui {
    class RecordWindow;
}

class RecordWindow : public QDialog
{
        Q_OBJECT

    public:
        explicit RecordWindow(QWidget *parent = 0);
        ~RecordWindow();

        QString getRecordDisplayName() const;
        QString getRecordId() const;
        QMap<QString, bool> getRecordFields() const;

        void clearRecordFields();

        void setRecordDisplayName(const QString& displayName);
        void setRecordId(const QString& id);
        void setRecordField(const QString& fieldId, const bool enabled);

    public slots:
        void accept();

    private slots:
        void on_lineEditDisplayName_textEdited(const QString &arg1);

    private:
        Ui::RecordWindow *ui;

        bool validate();
};

#endif // RECORDWINDOW_H
