#include "vector3rwidget.h"

#include <limits>

#include "../../Types/Model/vector.h"
#include "../../../Util/memoryutils.h"

using namespace Tome;


Vector3RWidget::Vector3RWidget(QWidget* parent)
    : QWidget(parent)
{
    // Create layout.
    this->layout = new QHBoxLayout();

    // Add widgets.
    this->spinBoxX = new QDoubleSpinBox();
    this->spinBoxX->setMinimum(-std::numeric_limits<float>::max());
    this->spinBoxX->setMaximum(std::numeric_limits<float>::max());
    this->spinBoxX->setDecimals(4);
    this->spinBoxX->setLocale(QLocale());
    this->layout->addWidget(this->spinBoxX);

    this->spinBoxY = new QDoubleSpinBox();
    this->spinBoxY->setMinimum(-std::numeric_limits<float>::max());
    this->spinBoxY->setMaximum(std::numeric_limits<float>::max());
    this->spinBoxY->setDecimals(4);
    this->spinBoxY->setLocale(QLocale());
    this->layout->addWidget(this->spinBoxY);

    this->spinBoxZ = new QDoubleSpinBox();
    this->spinBoxZ->setMinimum(-std::numeric_limits<float>::max());
    this->spinBoxZ->setMaximum(std::numeric_limits<float>::max());
    this->spinBoxZ->setDecimals(4);
    this->spinBoxZ->setLocale(QLocale());
    this->layout->addWidget(this->spinBoxZ);

    // Set layout.
    this->setLayout(this->layout);
    this->layout->setContentsMargins(0, 0, 0, 0);
}

Vector3RWidget::~Vector3RWidget()
{
    deleteLayout(this->layout);
}

QVariant Vector3RWidget::getValue() const
{
    QVariantMap map = QVariantMap();
    map[BuiltInType::Vector::X] = this->spinBoxX->value();
    map[BuiltInType::Vector::Y] = this->spinBoxY->value();
    map[BuiltInType::Vector::Z] = this->spinBoxZ->value();
    return map;
}

void Vector3RWidget::setValue(const QVariant& v)
{
    QVariantMap map = v.toMap();

    QVariant x = map[BuiltInType::Vector::X];
    QVariant y = map[BuiltInType::Vector::Y];
    QVariant z = map[BuiltInType::Vector::Z];

    this->spinBoxX->setValue(x.toDouble());
    this->spinBoxY->setValue(y.toDouble());
    this->spinBoxZ->setValue(z.toDouble());
}
