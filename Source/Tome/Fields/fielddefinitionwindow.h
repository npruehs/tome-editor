#ifndef FIELDDEFINITIONWINDOW_H
#define FIELDDEFINITIONWINDOW_H

#include <QDialog>

#include "../Projects/project.h"
#include "../Fields/fieldvaluewidget.h"


namespace Ui {
    class FieldDefinitionWindow;
}

class FieldDefinitionWindow : public QDialog
{
        Q_OBJECT

    public:
        explicit FieldDefinitionWindow(QWidget *parent = 0);
        ~FieldDefinitionWindow();

        QString getFieldComponent() const;
        QString getFieldDescription() const;
        QString getFieldDisplayName() const;
        QString getFieldId() const;
        QString getDefaultValue() const;
        QString getFieldType() const;

        void setFieldComponent(const QString& component) const;
        void setFieldDescription(const QString& description);
        void setFieldDisplayName(const QString& displayName);
        void setFieldId(const QString& fieldId);
        void setDefaultValue(const QString& defaultValue);
        void setFieldType(const QString& fieldType) const;

        void setProject(QSharedPointer<Tome::Project> project);

    public slots:
        void accept();

    private slots:
        void on_comboBoxType_currentIndexChanged(const QString &fieldType);
        void on_lineEditDisplayName_textEdited(const QString &displayName);

    private:
        QSharedPointer<Tome::Project> project;

        Ui::FieldDefinitionWindow *ui;
        Tome::FieldValueWidget* fieldValueWidget;

        bool validate();
};

#endif // FIELDDEFINITIONWINDOW_H
