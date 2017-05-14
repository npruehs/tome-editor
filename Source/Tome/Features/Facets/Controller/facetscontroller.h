#ifndef FACETSCONTROLLER_H
#define FACETSCONTROLLER_H

#include <QList>
#include <QVariant>

namespace Tome
{
    class Facet;
    class RecordsController;
    class TypesController;

    /**
     * @brief Controller for validating type facets.
     */
    class FacetsController
    {
        public:
            /**
             * @brief Constructs a new controller for validating type facets.
             * @param recordsController Controller for adding, updating and removing records.
             * @param typesController Controller for adding, updating and removing custom types.
             */
            FacetsController(const RecordsController& recordsController, const TypesController& typesController);
            ~FacetsController();

            /**
             * @brief Gets a list of all facets that may apply to the specified type.
             * @param targetType Type to get all facets for.
             * @return List of all facets that may apply to the specified type.
             */
            QList<Facet*> getFacets(const QString& targetType) const;

            /**
             * @brief Gets the restriction of the specified domain of the specified type (e.g. minimum value of an integer).
             * @param fieldType Type to get the restriction of.
             * @param facetKey Restriction to get.
             * @return Restriction of the specified domain of the specified type.
             */
            QVariant getFacetValue(const QString& fieldType, const QString& facetKey) const;

            /**
             * @brief Adds a new facet to be applied to custom types.
             * @param facet Facet to add.
             */
            void registerFacet(Facet* facet);

            /**
             * @brief Validates the specified field value against the domain restrictions imposed by the facets of its type.
             * @param fieldType Type of the field value to validate.
             * @param fieldValue Field value to validate.
             * @return Translated error message, if any validation errors occur, and an empty string otherwise.
             */
            QString validateFieldValue(const QString& fieldType, const QVariant& fieldValue) const;

        private:
            const RecordsController& recordsController;
            const TypesController& typesController;

            QList<Facet*> facets;
    };
}

#endif // FACETSCONTROLLER_H
