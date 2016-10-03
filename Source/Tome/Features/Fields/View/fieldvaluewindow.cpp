#include "fieldvaluewindow.h"
#include "ui_fieldvaluewindow.h"

#include <QMessageBox>
#include <QPushButton>

#include "fieldvaluewidget.h"
#include "../../Facets/Controller/facet.h"
#include "../../Records/Controller/recordscontroller.h"
#include "../../Types/Controller/typescontroller.h"
#include "../../Types/Model/customtype.h"

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

void FieldValueWindow::setFieldFacets(const QList<Facet*> facets, const QVariantMap& facetValues)
{
    this->facets = facets;
    this->facetValues = facetValues;
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
    if (this->validate())
    {
        this->done(Accepted);
    }
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
}

void FieldValueWindow::on_toolButtonRevert_clicked()
{
    emit revert();
}

bool FieldValueWindow::validate()
{
    // Validate all facets.
    for (int i = 0; i < this->facets.count(); ++i)
    {
        Facet* facet = this->facets[i];
        QString facetKey = facet->getKey();

        if (!this->facetValues.contains(facetKey))
        {
            continue;
        }

        QVariant facetValue = this->facetValues[facetKey];
        QVariant value = this->getFieldValue();

        QString validationError = facet->validateValue(value, facetValue);

        if (!validationError.isEmpty())
        {
            QString facetDisplayName = facet->getDisplayName();

            QMessageBox::information(
                        this,
                        facetDisplayName,
                        validationError,
                        QMessageBox::Close,
                        QMessageBox::Close);
            return false;
        }
    }

    return true;
}
