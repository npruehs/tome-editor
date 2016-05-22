#ifndef VECTOR2IWIDGET_H
#define VECTOR2IWIDGET_H

#include <QHBoxLayout>
#include <QSpinBox>
#include <QVariant>
#include <QWidget>


namespace Tome
{
    class Vector2IWidget : public QWidget
    {
            Q_OBJECT

        public:
            explicit Vector2IWidget(QWidget* parent = 0);
            ~Vector2IWidget();

            QVariant getValue() const;
            void setValue(const QVariant& v);

        private:
            QHBoxLayout* layout;

            QSpinBox* spinBoxX;
            QSpinBox* spinBoxY;
    };
}

#endif // VECTOR2IWIDGET_H
