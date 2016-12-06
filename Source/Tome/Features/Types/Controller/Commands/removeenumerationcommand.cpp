#include "removeenumerationcommand.h"

#include "../typescontroller.h"

using namespace Tome;


RemoveEnumerationCommand::RemoveEnumerationCommand(TypesController& typesController, const QString& name) :
    typesController(typesController),
    name(name)
{
    this->setText("Remove Enumeration - " + name);
}

void RemoveEnumerationCommand::undo()
{
    // Add custom type again.
    this->typesController.addEnumeration(this->name, this->enumeration, this->customTypeSetName);
}

void RemoveEnumerationCommand::redo()
{
    // Store current data.
    const CustomType& type = this->typesController.getCustomType(this->name);

    this->enumeration = type.getEnumeration();
    this->customTypeSetName = type.typeSetName;

    // Remove custom type.
    this->typesController.removeCustomType(this->name);
}
