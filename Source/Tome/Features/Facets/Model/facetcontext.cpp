#include "facetcontext.h"

#include "../../Records/Controller/recordscontroller.h"

using namespace Tome;


FacetContext::FacetContext(const RecordsController& recordsController)
    : recordsController(recordsController)
{
}
