#ifndef VECTOR3RWIDGET_H
#define VECTOR3RWIDGET_H


#include <QHBoxLayout>
#include <QSpinBox>
#include <QVariant>
#include <QWidget>


namespace Tome
{
    class Vector3RWidget : public QWidget
    {
            Q_OBJECT

        public:
            explicit Vector3RWidget(QWidget* parent = 0);
            ~Vector3RWidget();

            QVariant getValue() const;
            void setValue(const QVariant& v);

        private:
            QHBoxLayout* layout;

            QDoubleSpinBox* spinBoxX;
            QDoubleSpinBox* spinBoxY;
            QDoubleSpinBox* spinBoxZ;
    };
}

#endif // VECTOR3RWIDGET_H
