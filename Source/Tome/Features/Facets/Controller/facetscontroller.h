#ifndef FACETSCONTROLLER_H
#define FACETSCONTROLLER_H

#include <QList>

namespace Tome
{
    class Facet;
    class TypesController;

    class FacetsController
    {
        public:
            FacetsController(const TypesController& typesController);
            ~FacetsController();

            QList<Facet*> getFacets(const QString& targetType);
            void registerFacet(Facet* facet);

        private:
            const TypesController& typesController;

            QList<Facet*> facets;
    };
}

#endif // FACETSCONTROLLER_H
