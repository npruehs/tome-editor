#include "fieldvaluewindow.h"
#include "ui_fieldvaluewindow.h"

#include <QMessageBox>
#include <QPushButton>

#include "fieldvaluewidget.h"
#include "../../Facets/Controller/facet.h"
#include "../../Facets/Controller/facetscontroller.h"
#include "../../Facets/Model/facetcontext.h"
#include "../../Records/Controller/recordscontroller.h"
#include "../../Types/Controller/typescontroller.h"
#include "../../Types/Model/customtype.h"

using namespace Tome;


FieldValueWindow::FieldValueWindow(FacetsController& facetsController,
                                   ProjectController& projectController,
                                   RecordsController& recordsController,
                                   TypesController& typesController,
                                   QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FieldValueWindow),
    facetsController(facetsController),
    projectController(projectController),
    recordsController(recordsController),
    typesController(typesController)
{
    ui->setupUi(this);

    // Add widget for specifying the field value.
    this->fieldValueWidget = new FieldValueWidget(this->facetsController, this->projectController, this->recordsController, this->typesController, this);
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

void FieldValueWindow::setFieldCount(const int fieldCount)
{
    if (fieldCount > 1)
    {
        QString format = tr("You are editing the field values of %1 records.");
        QString message = format.arg(fieldCount);
        QString coloredMessage = QString("<font color=\"#FF0000\">%1</font>").arg(message);

        this->ui->labelMultiEditing->setText(coloredMessage);
        this->ui->labelMultiEditing->show();
    }
    else
    {
        this->ui->labelMultiEditing->clear();
        this->ui->labelMultiEditing->hide();
    }
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

void FieldValueWindow::accept()
{
    // Validate data.
    QString validationError = this->fieldValueWidget->validate();

    if (!validationError.isEmpty())
    {
        QMessageBox::information(
                    this,
                    tr("Invalid data"),
                    validationError,
                    QMessageBox::Close,
                    QMessageBox::Close);
        return;
    }

    this->done(Accepted);
}

void FieldValueWindow::showEvent(QShowEvent* event)
{
    QDialog::showEvent(event);

    // Enable user to input data immediately.
    this->fieldValueWidget->setFocus();

    // Set OK as default button (instead of Cancel).
    QPushButton* okButton = this->ui->buttonBox->button(QDialogButtonBox::Ok);
    okButton->setAutoDefault(true);
    okButton->setDefault(true);

    // Adjust size, so opening a field value window with large content
    // (e.g. color picker) does not cause huge empty areas in subsequent
    // field value windows with small content (e.g. checkbox).
    this->adjustSize();
}

void FieldValueWindow::on_toolButtonRevert_clicked()
{
    emit revert();
}
