#include "updateenumerationcommand.h"

#include "../typescontroller.h"

using namespace Tome;



UpdateEnumerationCommand::UpdateEnumerationCommand(TypesController& typesController,
                                                   const QString& oldName,
                                                   const QString& newName,
                                                   const QStringList& newEnumeration,
                                                   const QString& newTypeSetName)
    : typesController(typesController),
      oldName(oldName),
      newName(newName),
      newEnumeration(newEnumeration),
      newTypeSetName(newTypeSetName)
{
    this->setText(tr("Update Enumeration - %1").arg(oldName));
}

void UpdateEnumerationCommand::undo()
{
    // Restore custom type.
    this->typesController.updateEnumeration(this->newName,
                                            this->oldName,
                                            this->oldEnumeration,
                                            this->oldTypeSetName);
}


void UpdateEnumerationCommand::redo()
{
    // Store current data.
    const CustomType& type = this->typesController.getCustomType(this->oldName);

    this->oldEnumeration = type.getEnumeration();
    this->oldTypeSetName = type.typeSetName;

    // Update custom type.
    this->typesController.updateEnumeration(this->oldName,
                                            this->newName,
                                            this->newEnumeration,
                                            this->newTypeSetName);
}
