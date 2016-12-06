#include "addenumerationcommand.h"

#include "../typescontroller.h"

using namespace Tome;


AddEnumerationCommand::AddEnumerationCommand(TypesController& typesController,
                                             const QString& name,
                                             const QStringList& enumeration,
                                             const QString& customTypeSetName) :
    typesController(typesController),
    name(name),
    enumeration(enumeration),
    customTypeSetName(customTypeSetName)
{
    this->setText("Add Enumeration - " + name);
}

void AddEnumerationCommand::undo()
{
    this->typesController.removeCustomType(this->name);
}

void AddEnumerationCommand::redo()
{
    this->typesController.addEnumeration(this->name, this->enumeration, this->customTypeSetName);
}
