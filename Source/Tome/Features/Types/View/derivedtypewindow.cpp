#include "derivedtypewindow.h"
#include "ui_derivedtypewindow.h"

#include <QMessageBox>

#include "../../Facets/Controller/facet.h"
#include "../../Facets/Controller/facetscontroller.h"
#include "../../Facets/Model/facetcontext.h"
#include "../../Types/Controller/typescontroller.h"

using namespace Tome;

const int DerivedTypeWindow::FacetFormRow = 3;

DerivedTypeWindow::DerivedTypeWindow(Tome::TypesController& typesController,
                                     FacetsController& facetsController,
                                     Tome::RecordsController& recordsController,
        QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DerivedTypeWindow),
    typesController(typesController),
    facetsController(facetsController),
    recordsController(recordsController)
{
    ui->setupUi(this);
}

DerivedTypeWindow::~DerivedTypeWindow()
{
    delete ui;
}

QString DerivedTypeWindow::getBaseType() const
{
    return this->ui->comboBoxBaseType->currentText();
}

QVariantMap DerivedTypeWindow::getFacets() const
{
    QVariantMap facetMap;

    // Get all facets registered for the current type.
    QList<Facet*> typeFacets = this->facetsController.getFacets(this->getBaseType());

    for (int i = 0; i < typeFacets.count(); ++i)
    {
        // Get facet.
        Facet* facet = typeFacets[i];

        // Get current facet value.
        QString key = facet->getKey();
        QVariant value = facet->getWidgetValue(this->facetWidgets[i]);

        // Insert into facets map.
        facetMap.insert(key, value);
    }

    return facetMap;
}

QString DerivedTypeWindow::getTypeName() const
{
    return this->ui->lineEditName->text();
}

QString DerivedTypeWindow::getTypeSetName() const
{
    return this->ui->comboBoxTypeSet->currentText();
}

void DerivedTypeWindow::init()
{
    // Set type names.
    this->ui->comboBoxBaseType->clear();

    const QStringList& typeNames = this->typesController.getTypeNames();

    for (int i = 0; i < typeNames.length(); ++i)
    {
        this->ui->comboBoxBaseType->addItem(typeNames[i]);
    }

    // Set type set names.
    const QStringList typeSetNames = this->typesController.getCustomTypeSetNames();
    this->setTypeSetNames(typeSetNames);
    this->setTypeSetName(typeSetNames.first());
}

void DerivedTypeWindow::setBaseType(const QString& baseType) const
{
    this->ui->comboBoxBaseType->setCurrentText(baseType);
}

void DerivedTypeWindow::setFacets(const QVariantMap& facets)
{
    // Remove all existing facet widget from previous times this window was shown.
    QFormLayout* layout = static_cast<QFormLayout*>(this->layout());

    while (!this->facetWidgets.empty())
    {
        QWidget* facetWidget = this->facetWidgets.takeAt(0);
        QWidget* facetLabel = layout->labelForField(facetWidget);
        layout->removeWidget(facetLabel);
        layout->removeWidget(facetWidget);
        delete facetLabel;
        delete facetWidget;
    }

    // Add new facet widgets.
    QList<Facet*> typeFacets = this->facetsController.getFacets(this->getBaseType());
    FacetContext context = FacetContext(this->recordsController);

    for (int i = 0; i < typeFacets.count(); ++i)
    {
        // Create facet widget and label.
        Facet* facet = typeFacets[i];
        QString facetDisplayName = facet->getDisplayName();
        QWidget* facetWidget = facet->createWidget(context);

        // Get current facet value from field definition.
        QString facetKey = facet->getKey();
        QVariant facetValue = facets.contains(facetKey) ? facets[facetKey] : facet->getDefaultValue();
        facet->setWidgetValue(facetWidget, facetValue);

        // Insert into layout and remember for later removal.
        layout->insertRow(FacetFormRow + i, facetDisplayName + ":", facetWidget);
        this->facetWidgets.push_back(facetWidget);
    }
}

void DerivedTypeWindow::setTypeName(const QString& typeName)
{
    this->ui->lineEditName->setText(typeName);
}

void DerivedTypeWindow::setTypeSetName(const QString& typeSetName)
{
    this->ui->comboBoxTypeSet->setCurrentText(typeSetName);
}

void DerivedTypeWindow::setTypeSetNames(const QStringList& typeSetNames)
{
    this->ui->comboBoxTypeSet->clear();
    this->ui->comboBoxTypeSet->addItems(typeSetNames);
}

void DerivedTypeWindow::accept()
{
    // Validate data.
    if (this->validate())
    {
        this->done(Accepted);
    }
}

void DerivedTypeWindow::on_comboBoxBaseType_currentIndexChanged(const QString &baseType)
{
    Q_UNUSED(baseType)

    this->setFacets(QVariantMap());
}

bool DerivedTypeWindow::validate()
{
    // Name must not be empty.
    if (this->getTypeName().isEmpty())
    {
        QMessageBox::information(
                    this,
                    tr("Missing data"),
                    tr("Please specify a name for the derived type."),
                    QMessageBox::Close,
                    QMessageBox::Close);
        return false;
    }

    // Base type must not be empty.
    if (this->getBaseType().isEmpty())
    {
        QMessageBox::information(
                    this,
                    tr("Missing data"),
                    tr("Please specify a base type for the derived type."),
                    QMessageBox::Close,
                    QMessageBox::Close);
        return false;
    }

    return true;
}
