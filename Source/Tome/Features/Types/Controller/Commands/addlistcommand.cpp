#include "addlistcommand.h"

#include "../typescontroller.h"

using namespace Tome;


AddListCommand::AddListCommand(TypesController& typesController,
                               const QString& name,
                               const QString& itemType,
                               const QString& customTypeSetName) :
    typesController(typesController),
    name(name),
    itemType(itemType),
    customTypeSetName(customTypeSetName)
{
    this->setText(tr("Add List - %1").arg(name));
}

void AddListCommand::undo()
{
    this->typesController.removeCustomType(this->name);
}

void AddListCommand::redo()
{
    this->typesController.addList(this->name, this->itemType, this->customTypeSetName);
}
