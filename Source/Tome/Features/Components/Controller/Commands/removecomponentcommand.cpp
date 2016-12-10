#include "removecomponentcommand.h"

#include "../componentscontroller.h"
#include "../../../Fields/Controller/fielddefinitionscontroller.h"

using namespace Tome;


RemoveComponentCommand::RemoveComponentCommand(Tome::ComponentsController& componentsController, FieldDefinitionsController& fieldDefinitionsController, const QString& componentName) :
    componentsController(componentsController),
    fieldDefinitionsController(fieldDefinitionsController),
    componentName(componentName)
{
    this->setText(tr("Remove Component - %1").arg(componentName));
}

void RemoveComponentCommand::undo()
{
    // Add component again.
    this->componentsController.addComponent(this->componentName, this->componentSetName);

    // Restore component for all fields.
    for (int i = 0; i < this->affectedFields.count(); ++i)
    {
        const FieldDefinition& fieldDefinition = this->fieldDefinitionsController.getFieldDefinition(this->affectedFields[i].id);
        this->fieldDefinitionsController.updateFieldDefinition(
                    fieldDefinition.id,
                    fieldDefinition.id,
                    fieldDefinition.displayName,
                    fieldDefinition.fieldType,
                    fieldDefinition.defaultValue,
                    this->componentName,
                    fieldDefinition.description,
                    fieldDefinition.fieldDefinitionSetName);
    }
}

void RemoveComponentCommand::redo()
{
    // Store current data.
    this->componentSetName = this->componentsController.getSetNameOfComponent(componentName);
    this->affectedFields = this->fieldDefinitionsController.getFieldDefinitionsOfComponent(componentName);

    // Remove component.
    this->componentsController.removeComponent(componentName);
}
