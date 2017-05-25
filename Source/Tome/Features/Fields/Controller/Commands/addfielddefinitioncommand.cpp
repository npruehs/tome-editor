#include "addfielddefinitioncommand.h"

#include "../fielddefinitionscontroller.h"

using namespace Tome;

AddFieldDefinitionCommand::AddFieldDefinitionCommand(FieldDefinitionsController& fieldDefinitionsController,
                                                     const QString& id,
                                                     const QString& displayName,
                                                     const QString& fieldType,
                                                     const QVariant& defaultValue,
                                                     const QString& component,
                                                     const QString& description,
                                                     const QString& fieldDefinitionSetName) :
    fieldDefinitionsController(fieldDefinitionsController),
    id(id),
    displayName(displayName),
    fieldType(fieldType),
    defaultValue(defaultValue),
    component(component),
    description(description),
    fieldDefinitionSetName(fieldDefinitionSetName)
{
    this->setText(tr("Add Field - %1").arg(id));
}

void AddFieldDefinitionCommand::undo()
{
    qInfo(qUtf8Printable(QString("Undo add field %1.").arg(this->id)));

    this->fieldDefinitionsController.removeFieldDefinition(this->id);
}

void AddFieldDefinitionCommand::redo()
{
    this->fieldDefinitionsController.addFieldDefinition(this->id,
                                                        this->displayName,
                                                        this->fieldType,
                                                        this->defaultValue,
                                                        this->component,
                                                        this->description,
                                                        this->fieldDefinitionSetName);
}
