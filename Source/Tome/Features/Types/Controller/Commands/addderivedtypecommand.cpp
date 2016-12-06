#include "addderivedtypecommand.h"

#include "../typescontroller.h"

using namespace Tome;


AddDerivedTypeCommand::AddDerivedTypeCommand(TypesController& typesController,
                                             const QString& name,
                                             const QString& baseType,
                                             const QVariantMap& facets,
                                             const QString& customTypeSetName) :
    typesController(typesController),
    name(name),
    baseType(baseType),
    facets(facets),
    customTypeSetName(customTypeSetName)
{
    this->setText("Add Derived Type - " + name);
}

void AddDerivedTypeCommand::undo()
{
    this->typesController.removeCustomType(this->name);
}

void AddDerivedTypeCommand::redo()
{
    this->typesController.addDerivedType(this->name, this->baseType, this->facets, this->customTypeSetName);
}
