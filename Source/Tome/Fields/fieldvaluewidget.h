#ifndef FIELDVALUEWIDGET_H
#define FIELDVALUEWIDGET_H

#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QString>
#include <QVBoxLayout>
#include <QWidget>

#include "../Fields/fieldtype.h"

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

            FieldType::FieldType getFieldType() const;
            QString getFieldValue() const;

            void setFieldType(const FieldType::FieldType& fieldType);
            void setFieldValue(const QString& fieldValue);

        private:
            FieldType::FieldType fieldType;

            QCheckBox* checkBox;
            QDoubleSpinBox* doubleSpinBox;
            QVBoxLayout* layout;
            QLineEdit* lineEdit;
            QSpinBox* spinBox;
    };
}

#endif // FIELDVALUEWIDGET_H
