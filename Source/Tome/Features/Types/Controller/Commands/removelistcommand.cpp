#include "removelistcommand.h"

#include "../typescontroller.h"

using namespace Tome;

RemoveListCommand::RemoveListCommand(TypesController& typesController, const QString& name) :
    typesController(typesController),
    name(name)
{
    this->setText(tr("Remove List - %1").arg(name));
}

void RemoveListCommand::undo()
{
    // Add custom type again.
    this->typesController.addList(this->name, this->itemType, this->customTypeSetName);
}

void RemoveListCommand::redo()
{
    // Store current data.
    const CustomType& type = this->typesController.getCustomType(this->name);

    this->itemType = type.getItemType();
    this->customTypeSetName = type.typeSetName;

    // Remove custom type.
    this->typesController.removeCustomType(this->name);
}
