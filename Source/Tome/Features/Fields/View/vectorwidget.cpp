#include "vectorwidget.h"

#include <limits>

#include "../../Types/Model/vector.h"
#include "../../../Util/memoryutils.h"

using namespace Tome;


VectorWidget::VectorWidget(QWidget* parent)
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

    // Set layout.
    this->setLayout(this->layout);
    this->layout->setContentsMargins(0, 0, 0, 0);
}

VectorWidget::~VectorWidget()
{
    deleteLayout(this->layout);
}

QVariant VectorWidget::getValue() const
{
    QVariantMap map = QVariantMap();
    map[BuiltInType::Vector::X] = this->spinBoxX->value();
    map[BuiltInType::Vector::Y] = this->spinBoxY->value();
    return map;
}

void VectorWidget::setValue(const QVariant& v)
{
    QVariantMap map = v.toMap();

    QVariant x = map[BuiltInType::Vector::X];
    QVariant y = map[BuiltInType::Vector::Y];

    this->spinBoxX->setValue(x.toInt());
    this->spinBoxY->setValue(y.toInt());
}
