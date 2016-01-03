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
    /**
     * @brief Widget that changes its appearance depending on the specified type.
     */
    class FieldValueWidget : public QWidget
    {
            Q_OBJECT

        public:
            explicit FieldValueWidget(QWidget *parent = 0);
            ~FieldValueWidget();

            QString getFieldType() const;
            QString getFieldValue() const;

            void setFieldType(const QString& fieldType);
            void setFieldValue(const QString& fieldValue);
            void setEnumeration(const QStringList& enumeration);

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
            QComboBox* comboBox;
            QSpinBox* spinBox;

            void addWidget(QWidget* widget);
            void setCurrentWidget(QWidget* widget);
    };
}

#endif // FIELDVALUEWIDGET_H
