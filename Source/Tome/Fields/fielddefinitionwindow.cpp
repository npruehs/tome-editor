#include "fielddefinitionwindow.h"
#include "ui_fielddefinitionwindow.h"

#include <QMessageBox>

#include "../Fields/fieldtype.h"
#include "../Values/valueconverter.h"

using namespace Tome;


FieldDefinitionWindow::FieldDefinitionWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FieldDefinitionWindow)
{
    ui->setupUi(this);

    // Add supported field types.
    this->ui->comboBoxType->addItem("Int");
    this->ui->comboBoxType->addItem("String");
}

FieldDefinitionWindow::~FieldDefinitionWindow()
{
    delete ui;
}

void FieldDefinitionWindow::accept()
{
    // Validate data.
    if (this->validate())
    {
        this->done(Accepted);
    }
}

QString FieldDefinitionWindow::getFieldDescription() const
{
    return this->ui->plainTextEditDescription->toPlainText();
}

QString FieldDefinitionWindow::getFieldDisplayName() const
{
    return this->ui->lineEditDisplayName->text();
}

QString FieldDefinitionWindow::getFieldId() const
{
    return this->ui->lineEditId->text();
}

FieldType::FieldType FieldDefinitionWindow::getFieldType() const
{
    QString fieldType = this->ui->comboBoxType->currentText();
    QSharedPointer<ValueConverter> valueConverter = QSharedPointer<ValueConverter>::create();
    return valueConverter->StringToFieldType(fieldType);
}

void FieldDefinitionWindow::on_comboBoxType_currentIndexChanged(const QString &fieldType)
{
    FieldType::FieldType newType = this->getFieldType();

    switch (newType)
    {
        // TODO: Show new control with new default value.
    }
}

void FieldDefinitionWindow::on_lineEditDisplayName_textEdited(const QString &displayName)
{
    this->ui->lineEditId->setText(displayName);
}

bool FieldDefinitionWindow::validate()
{
    // Id must not be empty.
    if (this->getFieldId().size() == 0)
    {
        QMessageBox::information(
                    this,
                    tr("Missing data"),
                    tr("Please specify an id for the field."),
                    QMessageBox::Close,
                    QMessageBox::Close);
        return false;
    }

    // Display name must not be empty.
    if (this->getFieldDisplayName().size() == 0)
    {
        QMessageBox::information(
                    this,
                    tr("Missing data"),
                    tr("Please specify a name for the field."),
                    QMessageBox::Close,
                    QMessageBox::Close);
        return false;
    }

    return true;
}
