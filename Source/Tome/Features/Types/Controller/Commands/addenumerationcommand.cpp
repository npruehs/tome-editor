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
    this->setText(tr("Add Enumeration - %1").arg(name));
}

void AddEnumerationCommand::undo()
{
    qInfo(QString("Undo add enumeration %1.").arg(this->name).toUtf8().constData());

    this->typesController.removeCustomType(this->name);
}

void AddEnumerationCommand::redo()
{
    this->typesController.addEnumeration(this->name, this->enumeration, this->customTypeSetName);
}
