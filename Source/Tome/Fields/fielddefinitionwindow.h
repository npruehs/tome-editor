#ifndef FIELDDEFINITIONWINDOW_H
#define FIELDDEFINITIONWINDOW_H

#include <QDialog>

#include "../Fields/fieldvaluewidget.h"
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
        QString getDefaultValue() const;
        Tome::FieldType::FieldType getFieldType() const;

        void setFieldDescription(const QString& description);
        void setFieldDisplayName(const QString& displayName);
        void setFieldId(const QString& fieldId);
        void setDefaultValue(const QString& defaultValue);
        void setFieldType(const Tome::FieldType::FieldType& fieldType) const;
        void setRecordNames(const QStringList& recordNames);

    public slots:
        void accept();

    private slots:
        void on_comboBoxType_currentIndexChanged(const QString &fieldType);
        void on_lineEditDisplayName_textEdited(const QString &displayName);

    private:
        Ui::FieldDefinitionWindow *ui;
        Tome::FieldValueWidget* fieldValueWidget;

        bool validate();
};

#endif // FIELDDEFINITIONWINDOW_H
