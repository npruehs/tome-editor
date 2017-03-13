#include "labeledpixmapwidget.h"

#include "../../../Util/memoryutils.h"


using namespace Tome;


const int LabeledPixmapWidget::PixmapSize = 24;


LabeledPixmapWidget::LabeledPixmapWidget(QWidget* parent)
    : QWidget(parent)
{
    // Create layout.
    this->layout = new QHBoxLayout();

    // Add widgets.
    this->pixmapLabel = new QLabel();
    this->pixmapLabel->setMinimumWidth(PixmapSize);
    this->pixmapLabel->setMaximumWidth(PixmapSize);
    this->layout->addWidget(this->pixmapLabel);

    this->textLabel = new QLabel();
    this->layout->addWidget(this->textLabel);

    // Set layout.
    this->setLayout(this->layout);
    this->layout->setContentsMargins(0, 0, 0, 0);

}

LabeledPixmapWidget::~LabeledPixmapWidget()
{
    deleteLayout(this->layout);
}

QLabel& LabeledPixmapWidget::getTextLabel()
{
    return *this->textLabel;
}

QLabel& LabeledPixmapWidget::getPixmapLabel()
{
    return *this->pixmapLabel;
}

void LabeledPixmapWidget::setPixmap(const QPixmap& pixmap)
{
    QPixmap scaledPixmap = pixmap.isNull() ? pixmap : pixmap.scaled(QSize(PixmapSize, PixmapSize));
    this->pixmapLabel->setPixmap(scaledPixmap);
}

void LabeledPixmapWidget::setText(const QString& text)
{
    this->textLabel->setText(text);
}
