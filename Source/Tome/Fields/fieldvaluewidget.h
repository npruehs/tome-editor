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
            void setRecordNames(const QStringList& recordNames);

        private:
            FieldType::FieldType fieldType;

            QCheckBox* checkBox;
            QColorDialog* colorDialog;
            QDoubleSpinBox* doubleSpinBox;
            QVBoxLayout* layout;
            QLineEdit* lineEdit;
            QComboBox* referenceComboBox;
            QSpinBox* spinBox;
    };
}

#endif // FIELDVALUEWIDGET_H
