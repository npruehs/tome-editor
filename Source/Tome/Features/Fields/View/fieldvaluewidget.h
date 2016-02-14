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

#include "listwidget.h"
#include "../../Records/Controller/recordscontroller.h"
#include "../../Types/Controller/typescontroller.h"
#include "../../Types/Model/customtype.h"


namespace Tome
{
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
            QComboBox* comboBox;
            QSpinBox* spinBox;
            ListWidget* listWidget;

            RecordsController& recordsController;
            TypesController& typesController;

            void addWidget(QWidget* widget);
            void setCurrentWidget(QWidget* widget);
            void setEnumeration(const QStringList& enumeration);
    };
}

#endif // FIELDVALUEWIDGET_H
