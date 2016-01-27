#include "listwidget.h"

using namespace Tome;


ListWidget::ListWidget(QWidget *parent) :
    QWidget(parent),
    listItemWindow(0)
{
    // Create layout.
    this->layout = new QHBoxLayout(this);

    // Add list view.
    QStringList stringList;
    ListItemModel* model = new ListItemModel(stringList);
    this->viewModel = QSharedPointer<ListItemModel>(model);

    this->listView = new QListView(this);
    this->listView->setModel(model);
    this->layout->addWidget(this->listView);

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
    removeButton->setArrowType(Qt::RightArrow);
    buttonLayout->addWidget(removeButton);

    QToolButton* upButton = new QToolButton(this);
    upButton->setArrowType(Qt::UpArrow);
    buttonLayout->addWidget(upButton);

    QToolButton* downButton = new QToolButton(this);
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
        this->viewModel->addItem(value);
    }
}
