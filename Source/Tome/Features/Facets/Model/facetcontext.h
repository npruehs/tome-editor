#ifndef FACETCONTEXT_H
#define FACETCONTEXT_H

namespace Tome
{
    class RecordsController;

    /**
     * @brief Context to apply facets in.
     */
    class FacetContext
    {
        public:
            /**
             * @brief Constructs a new context to apply facets in.
             * @param recordsController Controller for adding, updating and removing records.
             */
            FacetContext(const RecordsController& recordsController);

            /**
             * @brief  Controller for adding, updating and removing records.
             */
            const RecordsController& recordsController;
    };
}

#endif // FACETCONTEXT_H
