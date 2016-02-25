#include "fieldvaluewindow.h"
#include "ui_fieldvaluewindow.h"

using namespace Tome;


FieldValueWindow::FieldValueWindow(RecordsController& recordsController, TypesController& typesController, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FieldValueWindow),
    recordsController(recordsController),
    typesController(typesController)
{
    ui->setupUi(this);

    // Add widget for specifying the field value.
    this->fieldValueWidget = new FieldValueWidget(this->recordsController, this->typesController, this);
    QGridLayout* layout = static_cast<QGridLayout*>(this->layout());
    layout->addWidget(this->fieldValueWidget, 3, 1);
}

FieldValueWindow::~FieldValueWindow()
{
    delete this->ui;

    delete this->fieldValueWidget;
}

QVariant FieldValueWindow::getFieldValue() const
{
    return this->fieldValueWidget->getFieldValue();
}

void FieldValueWindow::setFieldDescription(const QString& description)
{
    this->ui->labelDescriptionValue->setText(description);
}

void FieldValueWindow::setFieldDisplayName(const QString& displayName)
{
    this->ui->labelDisplayNameValue->setText(displayName);
}

void FieldValueWindow::setFieldValue(const QVariant& fieldValue)
{
    this->fieldValueWidget->setFieldValue(fieldValue);
}

void FieldValueWindow::setFieldType(const QString& fieldType) const
{
    this->ui->labelTypeValue->setText(fieldType);
    this->fieldValueWidget->setFieldType(fieldType);
}

void FieldValueWindow::showEvent(QShowEvent* event)
{
    QDialog::showEvent(event);
    this->fieldValueWidget->setFocus();
}

void FieldValueWindow::on_toolButtonRevert_clicked()
{
    emit revert();
}
