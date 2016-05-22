#ifndef VECTORWIDGET_H
#define VECTORWIDGET_H

#include <QHBoxLayout>
#include <QSpinBox>
#include <QVariant>
#include <QWidget>


namespace Tome
{
    class VectorWidget : public QWidget
    {
            Q_OBJECT

        public:
            explicit VectorWidget(QWidget* parent = 0);
            ~VectorWidget();

            QVariant getValue() const;
            void setValue(const QVariant& v);

        private:
            QHBoxLayout* layout;

            QSpinBox* spinBoxX;
            QSpinBox* spinBoxY;
    };
}

#endif // VECTORWIDGET_H
