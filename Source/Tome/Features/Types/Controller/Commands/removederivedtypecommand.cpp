#include "removederivedtypecommand.h"

#include "../typescontroller.h"

using namespace Tome;


RemoveDerivedTypeCommand::RemoveDerivedTypeCommand(TypesController& typesController, const QString& name) :
    typesController(typesController),
    name(name)
{
    this->setText("Remove Derived Type - " + name);
}

void RemoveDerivedTypeCommand::undo()
{
    // Add custom type again.
    this->typesController.addDerivedType(this->name, this->baseType, this->facets, this->customTypeSetName);
}

void RemoveDerivedTypeCommand::redo()
{
    // Store current data.
    const CustomType& type = this->typesController.getCustomType(this->name);

    this->baseType = type.getBaseType();
    this->facets = type.constrainingFacets;
    this->customTypeSetName = type.typeSetName;

    // Remove custom type.
    this->typesController.removeCustomType(this->name);
}
