#ifndef FACETSCONTROLLER_H
#define FACETSCONTROLLER_H

#include <QList>
#include <QVariant>

namespace Tome
{
    class Facet;
    class RecordsController;
    class TypesController;

    class FacetsController
    {
        public:
            FacetsController(const RecordsController& recordsController, const TypesController& typesController);
            ~FacetsController();

            QList<Facet*> getFacets(const QString& targetType);
            void registerFacet(Facet* facet);
            QString validateFieldValue(const QString& fieldType, const QVariant& fieldValue);

        private:
            const RecordsController& recordsController;
            const TypesController& typesController;

            QList<Facet*> facets;
    };
}

#endif // FACETSCONTROLLER_H
