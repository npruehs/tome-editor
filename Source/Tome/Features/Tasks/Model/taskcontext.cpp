#include "taskcontext.h"

#include "../../Components/Controller/componentscontroller.h"
#include "../../Fields/Controller/fielddefinitionscontroller.h"
#include "../../Records/Controller/recordscontroller.h"
#include "../../Types/Controller/typescontroller.h"

using namespace Tome;


TaskContext::TaskContext(const ComponentsController& componentsController, const FieldDefinitionsController& fieldDefinitionsController, const RecordsController& recordsController, const TypesController& typesController)
    : componentsController(componentsController),
      fieldDefinitionsController(fieldDefinitionsController),
      recordsController(recordsController),
      typesController(typesController)
{
}
