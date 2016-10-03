#ifndef FIELDDEFINITIONWINDOW_H
#define FIELDDEFINITIONWINDOW_H

#include <QDialog>
#include <QList>

#include "../../Components/Model/component.h"


namespace Ui {
    class FieldDefinitionWindow;
}

namespace Tome
{
    class ComponentsController;
    class FacetsController;
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
                Tome::FacetsController& facetsController,
                QWidget *parent = 0);
        ~FieldDefinitionWindow();

        Tome::Component getFieldComponent() const;
        QString getFieldDefinitionSetName() const;
        QString getFieldDescription() const;
        QString getFieldDisplayName() const;
        QVariantMap getFieldFacets() const;
        QString getFieldId() const;
        QVariant getDefaultValue() const;
        QString getFieldType() const;

        void init();

        void setFieldComponent(const QString& component) const;
        void setFieldDefinitionSetName(const QString& fieldDefinitionSetName);
        void setFieldDefinitionSetNames(const QStringList& fieldDefinitionSetNames);
        void setFieldDescription(const QString& description);
        void setFieldDisplayName(const QString& displayName);
        void setFieldFacets(const QVariantMap& facets);
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
        static const int DefaultFormRows;
        static const int FacetFormRow;
        static const int ValueFormRow;

        Ui::FieldDefinitionWindow *ui;
        Tome::FieldDefinitionsController& fieldDefinitionsController;
        Tome::ComponentsController& componentsController;
        Tome::RecordsController& recordsController;
        Tome::TypesController& typesController;
        Tome::FacetsController& facetsController;

        Tome::FieldValueWidget* fieldValueWidget;

        QList<QWidget*> facetWidgets;

        bool validate();
};

#endif // FIELDDEFINITIONWINDOW_H
