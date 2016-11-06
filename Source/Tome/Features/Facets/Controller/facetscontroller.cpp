#include "facetscontroller.h"

#include "facet.h"

#include "../../Types/Controller/typescontroller.h"

using namespace Tome;

FacetsController::FacetsController(const TypesController& typesController) :
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

QList<Facet*> FacetsController::getFacets(const QString& targetType)
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

void FacetsController::registerFacet(Tome::Facet* facet)
{
    qInfo(QString("Registering facet %1.").arg(facet->getKey()).toUtf8().constData());
    this->facets.push_back(facet);
}
