#ifndef VECTOR3IWIDGET_H
#define VECTOR3IWIDGET_H

#include <QHBoxLayout>
#include <QSpinBox>
#include <QVariant>
#include <QWidget>


namespace Tome
{
    /**
     * @brief Allows editing three-dimensional vectors with integer components.
     */
    class Vector3IWidget : public QWidget
    {
            Q_OBJECT

        public:
            /**
             * @brief Constructs a new widget for editing three-dimensional vectors with integer components.
             * @param parent Optional owner widget.
             */
            explicit Vector3IWidget(QWidget* parent = 0);
            ~Vector3IWidget();

            /**
             * @brief Returns the vector as QVariantMap with keys BuiltInType::Vector::X, BuiltInType::Vector::Y and BuiltInType::Vector::Z.
             * @return Current vector.
             */
            QVariant getValue() const;

            /**
             * @brief Sets the current vector.
             * @param v Vector as QVariantMap with keys BuiltInType::Vector::X, BuiltInType::Vector::Y and BuiltInType::Vector::Z.
             */
            void setValue(const QVariant& v);

        private:
            QHBoxLayout* layout;

            QSpinBox* spinBoxX;
            QSpinBox* spinBoxY;
            QSpinBox* spinBoxZ;
    };
}

#endif // VECTOR3IWIDGET_H
