#ifndef FIELDDEFINITIONWINDOW_H
#define FIELDDEFINITIONWINDOW_H

#include <QDialog>

#include "../../Components/Model/component.h"


namespace Ui {
    class FieldDefinitionWindow;
}

namespace Tome
{
    class ComponentsController;
    class FieldDefinitionsController;
    class FieldValueWidget;
    class RecordsController;
    class TypesController;
}

class FieldDefinitionWindow : public QDialog
{
        Q_OBJECT

    public:
        explicit FieldDefinitionWindow(
                Tome::FieldDefinitionsController& fieldDefinitionsController,
                Tome::ComponentsController& componentsController,
                Tome::RecordsController& recordsController,
                Tome::TypesController& typesController,
                QWidget *parent = 0);
        ~FieldDefinitionWindow();

        Tome::Component getFieldComponent() const;
        QString getFieldDefinitionSetName() const;
        QString getFieldDescription() const;
        QString getFieldDisplayName() const;
        QString getFieldId() const;
        QVariant getDefaultValue() const;
        QString getFieldType() const;

        void init();

        void setFieldComponent(const QString& component) const;
        void setFieldDefinitionSetName(const QString& fieldDefinitionSetName);
        void setFieldDefinitionSetNames(const QStringList& fieldDefinitionSetNames);
        void setFieldDescription(const QString& description);
        void setFieldDisplayName(const QString& displayName);
        void setFieldId(const QString& fieldId);
        void setDefaultValue(const QVariant& defaultValue);
        void setFieldType(const QString& fieldType) const;

    public slots:
        void accept();

    protected:
        void showEvent(QShowEvent * event);

    private slots:
        void on_comboBoxType_currentIndexChanged(const QString &fieldType);
        void on_lineEditDisplayName_textEdited(const QString &displayName);

    private:
        Ui::FieldDefinitionWindow *ui;
        Tome::FieldDefinitionsController& fieldDefinitionsController;
        Tome::ComponentsController& componentsController;
        Tome::RecordsController& recordsController;
        Tome::TypesController& typesController;

        Tome::FieldValueWidget* fieldValueWidget;

        bool validate();
};

#endif // FIELDDEFINITIONWINDOW_H
