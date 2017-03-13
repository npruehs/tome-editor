#include "facetscontroller.h"

#include "facet.h"

#include "../Model/facetcontext.h"

#include "../../Records/Controller/recordscontroller.h"
#include "../../Types/Controller/typescontroller.h"

using namespace Tome;


FacetsController::FacetsController(const RecordsController& recordsController, const TypesController& typesController) :
    recordsController(recordsController),
    typesController(typesController)
{
}

FacetsController::~FacetsController()
{
    for (int i = 0; i < this->facets.count(); ++i)
    {
        delete this->facets[i];
    }

    this->facets.clear();
}

QList<Facet*> FacetsController::getFacets(const QString& targetType) const
{
    // Check if derived type.
    if (this->typesController.isCustomType(targetType))
    {
        const CustomType& customType = this->typesController.getCustomType(targetType);

        if (customType.isDerivedType())
        {
            QString baseType = customType.getBaseType();
            return getFacets(baseType);
        }
    }

    // Get facets for built-in type.
    QList<Facet*> typeFacets;

    for (int i = 0; i < this->facets.count(); ++i)
    {
        if (this->facets[i]->getTargetType() == targetType)
        {
            typeFacets << facets[i];
        }
    }

    return typeFacets;
}

QVariant FacetsController::getFacetValue(const QString& fieldType, const QString& facetKey) const
{
    // Check if custom type.
    if (!this->typesController.isCustomType(fieldType))
    {
        return QVariant();
    }

    // Check if derived type.
    const CustomType& customType = this->typesController.getCustomType(fieldType);

    if (!customType.isDerivedType())
    {
        return QVariant();
    }

    for (int i = 0; i < this->facets.count(); ++i)
    {
        Facet* facet = this->facets[i];

        if (facet->getKey() != facetKey)
        {
            continue;
        }

        if (facet->getTargetType() != customType.getBaseType())
        {
            continue;
        }

        if (!customType.constrainingFacets.contains(facetKey))
        {
            continue;
        }

        return customType.constrainingFacets[facetKey];
    }

    return QVariant();
}

void FacetsController::registerFacet(Tome::Facet* facet)
{
    this->facets.push_back(facet);
}

QString FacetsController::validateFieldValue(const QString& fieldType, const QVariant& fieldValue) const
{
    // Check if custom type.
    if (!this->typesController.isCustomType(fieldType))
    {
        return QString();
    }

    // Check if derived type.
    const CustomType& customType = this->typesController.getCustomType(fieldType);

    if (!customType.isDerivedType())
    {
        return QString();
    }

    FacetContext context = FacetContext(this->recordsController);

    // Validate all facets.
    for (int i = 0; i < this->facets.count(); ++i)
    {
        Facet* facet = this->facets[i];
        QString facetKey = facet->getKey();

        if (facet->getTargetType() != customType.getBaseType())
        {
            continue;
        }

        if (!customType.constrainingFacets.contains(facetKey))
        {
            continue;
        }

        QVariant facetValue = customType.constrainingFacets[facetKey];

        QString validationError = facet->validateValue(context, fieldValue, facetValue);

        if (!validationError.isEmpty())
        {
            return validationError;
        }
    }

    return QString();
}
