#include "updatemapcommand.h"

#include "../typescontroller.h"

using namespace Tome;


UpdateMapCommand::UpdateMapCommand(TypesController& typesController,
                                   const QString& oldName,
                                   const QString& newName,
                                   const QString& newKeyType,
                                   const QString& newValueType,
                                   const QString& newTypeSetName)
    : typesController(typesController),
      oldName(oldName),
      newName(newName),
      newKeyType(newKeyType),
      newValueType(newValueType),
      newTypeSetName(newTypeSetName)
{
    this->setText("Update Map - " + oldName);
}

void UpdateMapCommand::undo()
{
    // Restore custom type.
    this->typesController.updateMap(this->newName,
                                    this->oldName,
                                    this->oldKeyType,
                                    this->oldValueType,
                                    this->oldTypeSetName);
}

void UpdateMapCommand::redo()
{
    // Store current data.
    const CustomType& type = this->typesController.getCustomType(this->oldName);

    this->oldKeyType = type.getKeyType();
    this->oldValueType = type.getValueType();
    this->oldTypeSetName = type.typeSetName;

    // Update custom type.
    this->typesController.updateMap(this->oldName,
                                    this->newName,
                                    this->newKeyType,
                                    this->newValueType,
                                    this->newTypeSetName);
}
