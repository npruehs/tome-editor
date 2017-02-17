#include "filepickerwidget.h"

#include <QFileDialog>

#include "../../../Util/memoryutils.h"

using namespace Tome;


FilePickerWidget::FilePickerWidget(QWidget* parent)
    : QWidget(parent)
{
    // Create layout.
    this->layout = new QHBoxLayout();

    // Add widgets.
    this->lineEdit = new QLineEdit();
    this->layout->addWidget(this->lineEdit);

    this->button = new QPushButton();
    this->button->setText(tr("Browse..."));
    this->layout->addWidget(this->button);

    // Set layout.
    this->setLayout(this->layout);
    this->layout->setContentsMargins(0, 0, 0, 0);

    // Connect signal.
    connect(
                this->button,
                SIGNAL(clicked(bool)),
                SLOT(onBrowseButtonClicked(bool))
                );
}

FilePickerWidget::~FilePickerWidget()
{
    deleteLayout(this->layout);
}

QVariant FilePickerWidget::getFileName() const
{
    return this->lineEdit->text();
}

void FilePickerWidget::setFileName(const QVariant& v)
{
    this->lineEdit->setText(v.toString());
}

void FilePickerWidget::onBrowseButtonClicked(bool checked)
{
    Q_UNUSED(checked)

    // Open file browser dialog.
    QString fileName = this->getFileName().toString();
    fileName = QFileDialog::getOpenFileName(this,
                                            tr("Select File"),
                                            fileName);

    // Set selected file.
    this->setFileName(fileName);
}
