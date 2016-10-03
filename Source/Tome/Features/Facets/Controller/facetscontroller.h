#ifndef FACETSCONTROLLER_H
#define FACETSCONTROLLER_H

#include <QList>

namespace Tome
{
    class Facet;

    class FacetsController
    {
        public:
            FacetsController();
            ~FacetsController();

            QList<Facet*> getFacets(const QString& targetType);
            void registerFacet(Facet* facet);

        private:
            QList<Facet*> facets;
    };
}

#endif // FACETSCONTROLLER_H
