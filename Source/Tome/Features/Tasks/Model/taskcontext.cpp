#include "taskcontext.h"

#include "../../Components/Controller/componentscontroller.h"
#include "../../Facets/Controller/facetscontroller.h"
#include "../../Fields/Controller/fielddefinitionscontroller.h"
#include "../../Records/Controller/recordscontroller.h"
#include "../../Types/Controller/typescontroller.h"

using namespace Tome;


TaskContext::TaskContext(
        const ComponentsController& componentsController,
        const FacetsController& facetsController,
        const FieldDefinitionsController& fieldDefinitionsController,
        const RecordsController& recordsController,
        const TypesController& typesController)
    : componentsController(componentsController),
      facetsController(facetsController),
      fieldDefinitionsController(fieldDefinitionsController),
      recordsController(recordsController),
      typesController(typesController)
{
}
