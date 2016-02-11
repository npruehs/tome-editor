#ifndef FIELDDEFINITIONWINDOW_H
#define FIELDDEFINITIONWINDOW_H

#include <QDialog>

#include "fieldvaluewidget.h"
#include "../Controller/fielddefinitionscontroller.h"
#include "../../Components/Controller/componentscontroller.h"
#include "../../Components/Model/component.h"
#include "../../Records/Controller/recordscontroller.h"
#include "../../Types/Controller/typescontroller.h"


namespace Ui {
    class FieldDefinitionWindow;
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

    public slots:
        void accept();
        int exec();

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
