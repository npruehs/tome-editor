#include "listwidget.h"

using namespace Tome;


ListWidget::ListWidget(QWidget *parent) :
    QWidget(parent),
    listItemWindow(0)
{
    // Create layout.
    this->layout = new QHBoxLayout(this);

    // Add list view.
    this->listWidget = new QListWidget(this);
    this->layout->addWidget(this->listWidget);

    // Add buttons.
    QVBoxLayout* buttonLayout = new QVBoxLayout(this);

    QToolButton* addButton = new QToolButton(this);
    addButton->setArrowType(Qt::LeftArrow);
    connect(
                addButton,
                SIGNAL(clicked()),
                SLOT(addItem())
                );
    buttonLayout->addWidget(addButton);

    QToolButton* removeButton = new QToolButton(this);
    connect(
                removeButton,
                SIGNAL(clicked()),
                SLOT(removeItem())
                );
    removeButton->setArrowType(Qt::RightArrow);
    buttonLayout->addWidget(removeButton);

    QToolButton* upButton = new QToolButton(this);
    connect(
                upButton,
                SIGNAL(clicked()),
                SLOT(moveItemUp())
                );
    upButton->setArrowType(Qt::UpArrow);
    buttonLayout->addWidget(upButton);

    QToolButton* downButton = new QToolButton(this);
    connect(
                downButton,
                SIGNAL(clicked()),
                SLOT(moveItemDown())
                );
    downButton->setArrowType(Qt::DownArrow);
    buttonLayout->addWidget(downButton);

    this->layout->addLayout(buttonLayout);

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

void ListWidget::addItem()
{
    // Prepare window.
    if (!this->listItemWindow)
    {
        this->listItemWindow = new ListItemWindow(this);
    }

    // Update view.
    this->listItemWindow->setFieldType(this->fieldType);

    // Show window.
    int result = this->listItemWindow->exec();

    if (result == QDialog::Accepted)
    {
        QString value = this->listItemWindow->getValue();

        // Update model.
        this->items.push_back(value);

        // Update view.
        this->listWidget->addItem(value);
    }
}

void ListWidget::removeItem()
{
    int index = this->getSelectedItemIndex();

    if (index < 0)
    {
        return;
    }

    // Update model.
    this->items.removeAt(index);

    // Update view.
    this->listWidget->takeItem(index);
}

void ListWidget::moveItemUp()
{
    int index = this->getSelectedItemIndex();

    if (index < 1)
    {
        return;
    }

    // Update model.
    this->items.move(index, index - 1);

    // Update view.
    QListWidgetItem* item = this->listWidget->takeItem(index);
    this->listWidget->insertItem(index - 1, item);

    // Update selection.
    this->listWidget->selectionModel()->select(
                this->listWidget->currentIndex().sibling(index - 1, 0),
                QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
}

void ListWidget::moveItemDown()
{
    int index = this->getSelectedItemIndex();

    if (index > this->items.size() - 2)
    {
        return;
    }

    // Update model.
    this->items.move(index, index + 1);

    // Update view.
    QListWidgetItem* item = this->listWidget->takeItem(index);
    this->listWidget->insertItem(index + 1, item);

    // Update selection.
    this->listWidget->selectionModel()->select(
                this->listWidget->currentIndex().sibling(index + 1, 0),
                QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
}

int ListWidget::getSelectedItemIndex() const
{
    QModelIndexList selectedIndexes = this->listWidget->selectionModel()->selectedRows();

    if (selectedIndexes.isEmpty())
    {
        return -1;
    }

    return selectedIndexes.first().row();
}
