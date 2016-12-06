#include "removemapcommand.h"

#include "../typescontroller.h"

using namespace Tome;

RemoveMapCommand::RemoveMapCommand(TypesController& typesController, const QString& name) :
    typesController(typesController),
    name(name)
{
    this->setText("Remove Map - " + name);
}

void RemoveMapCommand::undo()
{
    // Add custom type again.
    this->typesController.addMap(this->name, this->keyType, this->valueType, this->customTypeSetName);
}

void RemoveMapCommand::redo()
{
    // Store current data.
    const CustomType& type = this->typesController.getCustomType(this->name);

    this->keyType = type.getKeyType();
    this->valueType = type.getValueType();
    this->customTypeSetName = type.typeSetName;

    // Remove custom type.
    this->typesController.removeCustomType(this->name);
}
