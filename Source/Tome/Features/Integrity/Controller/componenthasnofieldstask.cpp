#include "componenthasnofieldstask.h"

#include <QSet>

#include "../../Components/Controller/componentscontroller.h"
#include "../../Fields/Controller/fielddefinitionscontroller.h"
#include "../../Tasks/Model/taskcontext.h"

using namespace Tome;


const QString ComponentHasNoFieldsTask::MessageCode = "TO0300";


ComponentHasNoFieldsTask::ComponentHasNoFieldsTask()
{
}

const QString ComponentHasNoFieldsTask::getDisplayName() const
{
    return MessageCode + tr(" - Component Has No Fields");
}

const MessageList ComponentHasNoFieldsTask::execute(const TaskContext& context) const
{
    MessageList messages;

    // Collect all used components.
    const FieldDefinitionList& fields = context.fieldDefinitionsController.getFieldDefinitions();

    QSet<Component> usedComponents;

    for (int i = 0; i < fields.count(); ++i)
    {
        const FieldDefinition& field = fields.at(i);
        usedComponents.insert(field.component);
    }

    // Check all component usage.
    const ComponentList& components = context.componentsController.getComponents();

    for (int i = 0; i < components.count(); ++i)
    {
        const Component& component = components.at(i);

        if (!usedComponents.contains(component))
        {
            Message message;
            message.content = tr("Component %1 has no fields.").arg(component);
            message.messageCode = MessageCode;
            message.severity = Severity::Warning;
            message.targetSiteId = component;
            message.targetSiteType = TargetSiteType::Component;

            messages.append(message);
        }
    }

    return messages;
}
