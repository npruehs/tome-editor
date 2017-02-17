#ifndef FIELDVALUEWIDGET_H
#define FIELDVALUEWIDGET_H

#include <QCheckBox>
#include <QColorDialog>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QString>
#include <QVBoxLayout>
#include <QVector>
#include <QWidget>

namespace Tome
{
    class CustomType;
    class FilePickerWidget;
    class ListWidget;
    class MapWidget;
    class FacetsController;
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
            explicit FieldValueWidget(FacetsController& facetsController, RecordsController& recordsController, TypesController& typesController,  QWidget *parent = 0);
            ~FieldValueWidget();

            QString getFieldType() const;
            QVariant getFieldValue() const;

            void setFieldType(const QString& fieldType);
            void setFieldValue(const QVariant& fieldValue);

            QString validate();

        protected:
            virtual void focusInEvent(QFocusEvent* event);

    private slots:
            void onColorDialogCurrentColorChanged(const QColor& color);
            void onDoubleSpinBoxValueChanged(double d);
            void onLineEditTextChanged(const QString& text);
            void onComboBoxCurrentIndexChanged(const QString& text);
            void onSpinBoxValueChanged(int i);

        private:
            QWidget* currentWidget;
            QString fieldType;

            QCheckBox* checkBox;
            QColorDialog* colorDialog;
            QDoubleSpinBox* doubleSpinBox;
            FilePickerWidget* filePicker;
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

            QLabel* errorLabel;

            FacetsController& facetsController;
            RecordsController& recordsController;
            TypesController& typesController;

            void addWidget(QWidget* widget);
            QVariant getFieldValueForType(const QString& typeName) const;
            void selectWidgetForType(const QString& typeName);
            void setCurrentWidget(QWidget* widget);
            void setEnumeration(const QStringList& enumeration);
            void setFieldValueForType(const QVariant& fieldValue, const QString& typeName);
            void updateErrorLabel();
    };
}

#endif // FIELDVALUEWIDGET_H
