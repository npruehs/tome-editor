#include "listwidget.h"

using namespace Tome;


ListWidget::ListWidget(QWidget *parent) :
    QWidget(parent)
{
    // Create layout.
    this->layout = new QHBoxLayout(this);

    // Add list view.
    this->listView = new QListView(this);
    this->layout->addWidget(this->listView);

    // Set layout.
    this->setLayout(this->layout);
    this->layout->setContentsMargins(0, 0, 0, 0);
}

ListWidget::~ListWidget()
{
}

QString ListWidget::getFieldType() const
{
    return this->fieldType;
}

void ListWidget::setFieldType(const QString& fieldType)
{
    this->fieldType = fieldType;
}
