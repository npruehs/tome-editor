#include "updatefielddefinitioncommand.h"

#include "../fielddefinitionscontroller.h"

using namespace Tome;


UpdateFieldDefinitionCommand::UpdateFieldDefinitionCommand(FieldDefinitionsController& fieldDefinitionsController,
                                                           const QString& oldId,
                                                           const QString& newId,
                                                           const QString& displayName,
                                                           const QString& fieldType,
                                                           const QVariant& defaultValue,
                                                           const QString& component,
                                                           const QString& description,
                                                           const QString& fieldDefinitionSetName)
    : fieldDefinitionsController(fieldDefinitionsController),
      oldId(oldId),
      newId(newId),
      newDisplayName(displayName),
      newFieldType(fieldType),
      newDefaultValue(defaultValue),
      newComponent(component),
      newDescription(description),
      newFieldDefinitionSetName(fieldDefinitionSetName)
{
    this->setText(tr("Update Field - %1").arg(oldId));
}

void UpdateFieldDefinitionCommand::undo()
{
    // Restore field definition.
    this->fieldDefinitionsController.updateFieldDefinition(this->newId,
                                                           this->oldId,
                                                           this->oldDisplayName,
                                                           this->oldFieldType,
                                                           this->oldDefaultValue,
                                                           this->oldComponent,
                                                           this->oldDescription,
                                                           this->oldFieldDefinitionSetName);
}

void UpdateFieldDefinitionCommand::redo()
{
    // Store current data.
    const FieldDefinition& fieldDefinition = this->fieldDefinitionsController.getFieldDefinition(this->oldId);

    this->oldDisplayName = fieldDefinition.displayName;
    this->oldFieldType = fieldDefinition.fieldType;
    this->oldDefaultValue = fieldDefinition.defaultValue;
    this->oldComponent = fieldDefinition.component;
    this->oldDescription = fieldDefinition.description;
    this->oldFieldDefinitionSetName = fieldDefinition.fieldDefinitionSetName;

    // Update field definition.
    this->fieldDefinitionsController.updateFieldDefinition(this->oldId,
                                                           this->newId,
                                                           this->newDisplayName,
                                                           this->newFieldType,
                                                           this->newDefaultValue,
                                                           this->newComponent,
                                                           this->newDescription,
                                                           this->newFieldDefinitionSetName);
}
