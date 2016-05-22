#include "vector3iwidget.h"

#include <limits>

#include "../../Types/Model/vector.h"
#include "../../../Util/memoryutils.h"

using namespace Tome;


Vector3IWidget::Vector3IWidget(QWidget* parent)
    : QWidget(parent)
{
    // Create layout.
    this->layout = new QHBoxLayout();

    // Add widgets.
    this->spinBoxX = new QSpinBox();
    this->spinBoxX->setMinimum(std::numeric_limits<int>::min());
    this->spinBoxX->setMaximum(std::numeric_limits<int>::max());
    this->layout->addWidget(this->spinBoxX);

    this->spinBoxY = new QSpinBox();
    this->spinBoxY->setMinimum(std::numeric_limits<int>::min());
    this->spinBoxY->setMaximum(std::numeric_limits<int>::max());
    this->layout->addWidget(this->spinBoxY);

    this->spinBoxZ = new QSpinBox();
    this->spinBoxZ->setMinimum(std::numeric_limits<int>::min());
    this->spinBoxZ->setMaximum(std::numeric_limits<int>::max());
    this->layout->addWidget(this->spinBoxZ);

    // Set layout.
    this->setLayout(this->layout);
    this->layout->setContentsMargins(0, 0, 0, 0);
}

Vector3IWidget::~Vector3IWidget()
{
    deleteLayout(this->layout);
}

QVariant Vector3IWidget::getValue() const
{
    QVariantMap map = QVariantMap();
    map[BuiltInType::Vector::X] = this->spinBoxX->value();
    map[BuiltInType::Vector::Y] = this->spinBoxY->value();
    map[BuiltInType::Vector::Z] = this->spinBoxZ->value();
    return map;
}

void Vector3IWidget::setValue(const QVariant& v)
{
    QVariantMap map = v.toMap();

    QVariant x = map[BuiltInType::Vector::X];
    QVariant y = map[BuiltInType::Vector::Y];
    QVariant z = map[BuiltInType::Vector::Z];

    this->spinBoxX->setValue(x.toInt());
    this->spinBoxY->setValue(y.toInt());
    this->spinBoxZ->setValue(z.toInt());
}
