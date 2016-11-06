#ifndef FIELDVALUEWIDGET_H
#define FIELDVALUEWIDGET_H

#include <QCheckBox>
#include <QColorDialog>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QString>
#include <QVBoxLayout>
#include <QVector>
#include <QWidget>

namespace Tome
{
    class CustomType;
    class ListWidget;
    class MapWidget;
    class RecordsController;
    class TypesController;
    class Vector2IWidget;
    class Vector2RWidget;
    class Vector3IWidget;
    class Vector3RWidget;
    class Facet;

    /**
     * @brief Changes its appearance depending on the specified type.
     */
    class FieldValueWidget : public QWidget
    {
            Q_OBJECT

        public:
            explicit FieldValueWidget(RecordsController& recordsController, TypesController& typesController, QWidget *parent = 0);
            ~FieldValueWidget();

            QString getFieldType() const;
            QVariant getFieldValue() const;

            void setFieldType(const QString& fieldType);
            void setFieldValue(const QVariant& fieldValue);
            void setFieldFacets(const QList<Facet*> &facets, const QVariantMap &facetValues);

        protected:
            virtual void focusInEvent(QFocusEvent* event);

        private:
            QWidget* currentWidget;
            QString fieldType;
            QList<Tome::Facet*> facets;
            QVariantMap facetValues;

            QCheckBox* checkBox;
            QColorDialog* colorDialog;
            QDoubleSpinBox* doubleSpinBox;
            QVBoxLayout* layout;
            QLineEdit* lineEdit;
            ListWidget* listWidget;
            MapWidget* mapWidget;
            QComboBox* comboBox;
            QSpinBox* spinBox;
            Vector2IWidget* vector2IWidget;
            Vector3IWidget* vector3IWidget;
            Vector2RWidget* vector2RWidget;
            Vector3RWidget* vector3RWidget;

            RecordsController& recordsController;
            TypesController& typesController;

            void addWidget(QWidget* widget);
            QVariant getFieldValueForType(const QString& typeName) const;
            void selectWidgetForType(const QString& typeName);
            void setCurrentWidget(QWidget* widget);
            void setEnumeration(const QStringList& enumeration);
            void setFieldValueForType(const QVariant& fieldValue, const QString& typeName);
    };
}

#endif // FIELDVALUEWIDGET_H
