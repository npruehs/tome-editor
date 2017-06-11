#include "mapvaluetypedoesnotexisttask.h"

#include "../../Tasks/Model/taskcontext.h"
#include "../../Types/Controller/typescontroller.h"

using namespace Tome;


const QString MapValueTypeDoesNotExistTask::MessageCode = "TO0104";


const QString MapValueTypeDoesNotExistTask::getDisplayName() const
{
    return MessageCode + tr(" - Map Value Type Does Not Exist");
}

const MessageList MapValueTypeDoesNotExistTask::execute(const TaskContext& context) const
{
    MessageList messages;

    // Check all types.
    const CustomTypeList& types = context.typesController.getCustomTypes();

    for (int i = 0; i < types.count(); ++i)
    {
        const CustomType& type = types.at(i);

        if (type.isMap())
        {
            const QString valueType = type.getValueType();

            // Check map key type.
            if (!context.typesController.isBuiltInType(valueType) && !context.typesController.isCustomType(valueType))
            {
                Message message;
                message.content = tr("The value type %1 of the map type %2 does not exist.").arg(valueType, type.name);
                message.messageCode = MessageCode;
                message.severity = Severity::Error;
                message.targetSiteId = type.name;
                message.targetSiteType = TargetSiteType::Type;

                messages.append(message);
            }
        }
    }

    return messages;
}
