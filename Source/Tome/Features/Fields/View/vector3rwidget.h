#ifndef VECTOR3RWIDGET_H
#define VECTOR3RWIDGET_H


#include <QHBoxLayout>
#include <QSpinBox>
#include <QVariant>
#include <QWidget>


namespace Tome
{
    /**
     * @brief Allows editing three-dimensional vectors with real components.
     */
    class Vector3RWidget : public QWidget
    {
            Q_OBJECT

        public:
            /**
             * @brief Constructs a new widget for editing three-dimensional vectors with real components.
             * @param parent Optional owner widget.
             */
            explicit Vector3RWidget(QWidget* parent = 0);
            ~Vector3RWidget();

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

            QDoubleSpinBox* spinBoxX;
            QDoubleSpinBox* spinBoxY;
            QDoubleSpinBox* spinBoxZ;
    };
}

#endif // VECTOR3RWIDGET_H
