#include "facetscontroller.h"

#include "facet.h"

using namespace Tome;

FacetsController::FacetsController()
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
    this->facets.push_back(facet);
}
