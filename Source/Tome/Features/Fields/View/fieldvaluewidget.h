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
    class RecordsController;
    class TypesController;
    class Vector2IWidget;
    class Vector2RWidget;
    class Vector3IWidget;
    class Vector3RWidget;

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

        protected:
            virtual void focusInEvent(QFocusEvent* event);

        private:
            QWidget* currentWidget;
            QString fieldType;

            QCheckBox* checkBox;
            QColorDialog* colorDialog;
            QDoubleSpinBox* doubleSpinBox;
            QVBoxLayout* layout;
            QLineEdit* lineEdit;
            ListWidget* listWidget;
            QComboBox* comboBox;
            QSpinBox* spinBox;
            Vector2IWidget* vector2IWidget;
            Vector3IWidget* vector3IWidget;
            Vector2RWidget* vector2RWidget;
            Vector3RWidget* vector3RWidget;

            RecordsController& recordsController;
            TypesController& typesController;

            void addWidget(QWidget* widget);
            void setCurrentWidget(QWidget* widget);
            void setEnumeration(const QStringList& enumeration);
    };
}

#endif // FIELDVALUEWIDGET_H
