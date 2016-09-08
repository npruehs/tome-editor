#ifndef VECTOR2RWIDGET_H
#define VECTOR2RWIDGET_H

#include <QDoubleSpinBox>
#include <QHBoxLayout>
#include <QVariant>
#include <QWidget>


namespace Tome
{
    class Vector2RWidget : public QWidget
    {
            Q_OBJECT

        public:
            explicit Vector2RWidget(QWidget* parent = 0);
            ~Vector2RWidget();

            QVariant getValue() const;
            void setValue(const QVariant& v);

        private:
            QHBoxLayout* layout;

            QDoubleSpinBox* spinBoxX;
            QDoubleSpinBox* spinBoxY;
    };
}

#endif // VECTOR2RWIDGET_H
