#include "updatelistcommand.h"

#include "../typescontroller.h"

using namespace Tome;



UpdateListCommand::UpdateListCommand(TypesController& typesController,
                                     const QString& oldName,
                                     const QString& newName,
                                     const QString& newItemType,
                                     const QString& newTypeSetName)
    : typesController(typesController),
      oldName(oldName),
      newName(newName),
      newItemType(newItemType),
      newTypeSetName(newTypeSetName)
{
    this->setText(tr("Update List - %1").arg(oldName));
}

void UpdateListCommand::undo()
{
    qInfo(qUtf8Printable(QString("Undo update list %1.").arg(this->oldName)));

    // Restore custom type.
    this->typesController.updateList(this->newName,
                                     this->oldName,
                                     this->oldItemType,
                                     this->oldTypeSetName);
}

void UpdateListCommand::redo()
{
    // Store current data.
    const CustomType& type = this->typesController.getCustomType(this->oldName);

    this->oldItemType = type.getItemType();
    this->oldTypeSetName = type.typeSetName;

    // Update custom type.
    this->typesController.updateList(this->oldName,
                                     this->newName,
                                     this->newItemType,
                                     this->newTypeSetName);
}
