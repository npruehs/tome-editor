#ifndef FIELDDEFINITIONWINDOW_H
#define FIELDDEFINITIONWINDOW_H

#include <QCloseEvent>
#include <QDialog>

#include "../Fields/fieldtype.h"


namespace Ui {
    class FieldDefinitionWindow;
}

class FieldDefinitionWindow : public QDialog
{
        Q_OBJECT

    public:
        explicit FieldDefinitionWindow(QWidget *parent = 0);
        ~FieldDefinitionWindow();

        QString getFieldDescription() const;
        QString getFieldDisplayName() const;
        QString getFieldId() const;
        Tome::FieldType::FieldType getFieldType() const;

    public slots:
        void accept();

    protected:
        void closeEvent(QCloseEvent *event);

    private slots:
        void on_comboBoxType_currentIndexChanged(const QString &arg1);
        void on_lineEditDisplayName_textEdited(const QString &arg1);

    private:
        Ui::FieldDefinitionWindow *ui;

        bool validate();
};

#endif // FIELDDEFINITIONWINDOW_H
