#include "typeisneverusedtask.h"

#include <QSet>

#include "../../Fields/Controller/fielddefinitionscontroller.h"
#include "../../Tasks/Model/taskcontext.h"
#include "../../Types/Controller/typescontroller.h"

using namespace Tome;


const QString TypeIsNeverUsedTask::MessageCode = "TO0302";


const QString TypeIsNeverUsedTask::getDisplayName() const
{
    return MessageCode + tr(" - Type Is Never Used");
}

const MessageList TypeIsNeverUsedTask::execute(const TaskContext& context) const
{
    MessageList messages;

    // Collect all used types.
    const FieldDefinitionList& fields = context.fieldDefinitionsController.getFieldDefinitions();

    QSet<QString> usedTypes;

    // Add type references by fields.
    for (int i = 0; i < fields.count(); ++i)
    {
        const FieldDefinition& field = fields.at(i);
        usedTypes.insert(field.fieldType);
    }

    // Add type references by other types.
    const CustomTypeList& types = context.typesController.getCustomTypes();

    for (int i = 0; i < types.count(); ++i)
    {
        const CustomType& type = types.at(i);

        if (type.isDerivedType())
        {
            usedTypes.insert(type.getBaseType());
        }
        else if (type.isList())
        {
            usedTypes.insert(type.getItemType());
        }
        else if (type.isMap())
        {
            usedTypes.insert(type.getKeyType());
            usedTypes.insert(type.getValueType());
        }
    }

    // Check all type usage.
    for (int i = 0; i < types.count(); ++i)
    {
        const CustomType& type = types.at(i);

        if (!usedTypes.contains(type.name))
        {
            Message message;
            message.content = tr("The type %1 is defined but never used.").arg(type.name);
            message.messageCode = MessageCode;
            message.severity = Severity::Warning;
            message.targetSiteId = type.name;
            message.targetSiteType = TargetSiteType::Type;

            messages.append(message);
        }
    }

    return messages;
}
