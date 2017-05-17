#ifndef VECTOR2IWIDGET_H
#define VECTOR2IWIDGET_H

#include <QHBoxLayout>
#include <QSpinBox>
#include <QVariant>
#include <QWidget>


namespace Tome
{
    /**
     * @brief Allows editing two-dimensional vectors with integer components.
     */
    class Vector2IWidget : public QWidget
    {
            Q_OBJECT

        public:
            /**
             * @brief Constructs a new widget for editing two-dimensional vectors with integer components.
             * @param parent Optional owner widget.
             */
            explicit Vector2IWidget(QWidget* parent = 0);
            ~Vector2IWidget();

            /**
             * @brief Returns the vector as QVariantMap with keys BuiltInType::Vector::X and BuiltInType::Vector::Y.
             * @return Current vector.
             */
            QVariant getValue() const;

            /**
             * @brief Sets the current vector.
             * @param v Vector as QVariantMap with keys BuiltInType::Vector::X and BuiltInType::Vector::Y.
             */
            void setValue(const QVariant& v);

        private:
            QHBoxLayout* layout;

            QSpinBox* spinBoxX;
            QSpinBox* spinBoxY;
    };
}

#endif // VECTOR2IWIDGET_H
