#ifndef FACETCONTEXT_H
#define FACETCONTEXT_H

namespace Tome
{
    class RecordsController;

    class FacetContext
    {
        public:
            FacetContext(const RecordsController& recordsController);

            const RecordsController& recordsController;
    };
}

#endif // FACETCONTEXT_H
