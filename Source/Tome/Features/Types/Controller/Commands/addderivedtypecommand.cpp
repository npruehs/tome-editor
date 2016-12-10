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
    this->setText(tr("Add Derived Type - %1").arg(name));
}

void AddDerivedTypeCommand::undo()
{
    qInfo(QString("Undo add derived type %1.").arg(this->name).toUtf8().constData());

    this->typesController.removeCustomType(this->name);
}

void AddDerivedTypeCommand::redo()
{
    this->typesController.addDerivedType(this->name, this->baseType, this->facets, this->customTypeSetName);
}
