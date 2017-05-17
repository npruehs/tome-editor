#ifndef VECTOR2RWIDGET_H
#define VECTOR2RWIDGET_H

#include <QDoubleSpinBox>
#include <QHBoxLayout>
#include <QVariant>
#include <QWidget>


namespace Tome
{
    /**
     * @brief Allows editing two-dimensional vectors with real components.
     */
    class Vector2RWidget : public QWidget
    {
            Q_OBJECT

        public:
            /**
             * @brief Constructs a new widget for editing two-dimensional vectors with real components.
             * @param parent Optional owner widget.
             */
            explicit Vector2RWidget(QWidget* parent = 0);
            ~Vector2RWidget();

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

            QDoubleSpinBox* spinBoxX;
            QDoubleSpinBox* spinBoxY;
    };
}

#endif // VECTOR2RWIDGET_H
