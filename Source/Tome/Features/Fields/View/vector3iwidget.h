#ifndef VECTOR3IWIDGET_H
#define VECTOR3IWIDGET_H

#include <QHBoxLayout>
#include <QSpinBox>
#include <QVariant>
#include <QWidget>


namespace Tome
{
    class Vector3IWidget : public QWidget
    {
            Q_OBJECT

        public:
            explicit Vector3IWidget(QWidget* parent = 0);
            ~Vector3IWidget();

            QVariant getValue() const;
            void setValue(const QVariant& v);

        private:
            QHBoxLayout* layout;

            QSpinBox* spinBoxX;
            QSpinBox* spinBoxY;
            QSpinBox* spinBoxZ;
    };
}

#endif // VECTOR3IWIDGET_H
