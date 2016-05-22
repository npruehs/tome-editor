#include "vector2rwidget.h"

#include <limits>

#include "../../Types/Model/vector.h"
#include "../../../Util/memoryutils.h"

using namespace Tome;


Vector2RWidget::Vector2RWidget(QWidget* parent)
    : QWidget(parent)
{
    // Create layout.
    this->layout = new QHBoxLayout();

    // Add widgets.
    this->spinBoxX = new QDoubleSpinBox();
    this->spinBoxX->setMinimum(std::numeric_limits<float>::min());
    this->spinBoxX->setMaximum(std::numeric_limits<float>::max());
    this->spinBoxX->setDecimals(3);
    this->layout->addWidget(this->spinBoxX);

    this->spinBoxY = new QDoubleSpinBox();
    this->spinBoxY->setMinimum(std::numeric_limits<float>::min());
    this->spinBoxY->setMaximum(std::numeric_limits<float>::max());
    this->spinBoxY->setDecimals(3);
    this->layout->addWidget(this->spinBoxY);

    // Set layout.
    this->setLayout(this->layout);
    this->layout->setContentsMargins(0, 0, 0, 0);
}

Vector2RWidget::~Vector2RWidget()
{
    deleteLayout(this->layout);
}

QVariant Vector2RWidget::getValue() const
{
    QVariantMap map = QVariantMap();
    map[BuiltInType::Vector::X] = this->spinBoxX->value();
    map[BuiltInType::Vector::Y] = this->spinBoxY->value();
    return map;
}

void Vector2RWidget::setValue(const QVariant& v)
{
    QVariantMap map = v.toMap();

    QVariant x = map[BuiltInType::Vector::X];
    QVariant y = map[BuiltInType::Vector::Y];

    this->spinBoxX->setValue(x.toDouble());
    this->spinBoxY->setValue(y.toDouble());
}
