#include "updatederivedtypecommand.h"

#include "../typescontroller.h"

using namespace Tome;


UpdateDerivedTypeCommand::UpdateDerivedTypeCommand(TypesController& typesController,
                                                   const QString& oldName,
                                                   const QString& newName,
                                                   const QString& baseType,
                                                   const QVariantMap facets,
                                                   const QString& typeSetName)
    : typesController(typesController),
      oldName(oldName),
      newName(newName),
      newBaseType(baseType),
      newFacets(facets),
      newTypeSetName(typeSetName)
{
    this->setText(tr("Update Derived Type - %1").arg(oldName));
}

void UpdateDerivedTypeCommand::undo()
{
    qInfo(QString("Undo update derived type %1.").arg(this->oldName).toUtf8().constData());

    // Restore custom type.
    this->typesController.updateDerivedType(this->newName,
                                            this->oldName,
                                            this->oldBaseType,
                                            this->oldFacets,
                                            this->oldTypeSetName);
}

void UpdateDerivedTypeCommand::redo()
{
    // Store current data.
    const CustomType& type = this->typesController.getCustomType(this->oldName);

    this->oldBaseType = type.getBaseType();
    this->oldFacets = type.constrainingFacets;
    this->oldTypeSetName = type.typeSetName;

    // Update custom type.
    this->typesController.updateDerivedType(this->oldName,
                                            this->newName,
                                            this->newBaseType,
                                            this->newFacets,
                                            this->newTypeSetName);
}
