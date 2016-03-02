#include "listwidget.h"

#include "listitemwindow.h"
#include "../../Records/Controller/recordscontroller.h"
#include "../../Types/Controller/typescontroller.h"
#include "../../../Util/memoryutils.h"


using namespace Tome;


ListWidget::ListWidget(RecordsController& recordsController, TypesController& typesController, QWidget *parent) :
    QWidget(parent),
    listItemWindow(0),
    recordsController(recordsController),
    typesController(typesController)
{
    // Create layout.
    this->layout = new QHBoxLayout(this);

    // Add list view.
    this->listWidget = new QListWidget(this);
    connect(
                this->listWidget,
                SIGNAL(itemDoubleClicked(QListWidgetItem*)),
                SLOT(editItem(QListWidgetItem*))
                );
    this->layout->addWidget(this->listWidget);

    // Add buttons.
    this->buttonLayout = new QVBoxLayout();

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
    deleteLayout(this->buttonLayout);
    deleteLayout(this->layout);
}

QString ListWidget::getFieldType() const
{
    return this->fieldType;
}

QVariantList ListWidget::getItems() const
{
    return this->items;
}

void ListWidget::setFieldType(const QString& fieldType)
{
    this->fieldType = fieldType;
}

void ListWidget::setItems(const QVariantList& items)
{
    // Update model.
    this->items = items;

    // Update view.
    this->listWidget->clear();

    for (int i = 0; i < items.size(); ++i)
    {
        this->listWidget->addItem(items[i].toString());
    }
}

void ListWidget::addItem()
{
    // Prepare window.
    if (!this->listItemWindow)
    {
        this->listItemWindow = new ListItemWindow(this->recordsController, this->typesController, this);
    }

    // Update view.
    this->listItemWindow->setFieldType(this->fieldType);

    // Show window.
    int result = this->listItemWindow->exec();

    if (result == QDialog::Accepted)
    {
        QVariant value = this->listItemWindow->getValue();

        // Update model.
        this->items.push_back(value);

        // Update view.
        this->listWidget->addItem(value.toString());
    }
}

void ListWidget::editItem(QListWidgetItem* item)
{
    // Prepare window.
    if (!this->listItemWindow)
    {
        this->listItemWindow = new ListItemWindow(this->recordsController, this->typesController, this);
    }

    QVariant currentValue = item->text();

    // Update view.
    this->listItemWindow->setFieldType(this->fieldType);
    this->listItemWindow->setValue(currentValue);

    // Show window.
    int result = this->listItemWindow->exec();

    if (result == QDialog::Accepted)
    {
        QVariant value = this->listItemWindow->getValue();

        // Update model.
        int index = this->getSelectedItemIndex();
        this->items[index] = value;

        // Update view.
        item->setText(value.toString());
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
