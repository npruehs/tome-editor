#include "mapkeytypedoesnotexisttask.h"

#include "../../Tasks/Model/taskcontext.h"
#include "../../Types/Controller/typescontroller.h"

using namespace Tome;


const QString MapKeyTypeDoesNotExistTask::MessageCode = "TO0103";


MapKeyTypeDoesNotExistTask::MapKeyTypeDoesNotExistTask()
{
}

const QString MapKeyTypeDoesNotExistTask::getDisplayName() const
{
    return MessageCode + tr(" - Map Key Type Does Not Exist");
}

const MessageList MapKeyTypeDoesNotExistTask::execute(const TaskContext& context) const
{
    MessageList messages;

    // Check all types.
    const CustomTypeList& types = context.typesController.getCustomTypes();

    for (int i = 0; i < types.count(); ++i)
    {
        const CustomType& type = types.at(i);

        if (type.isMap())
        {
            const QString keyType = type.getKeyType();

            // Check map key type.
            if (!context.typesController.isBuiltInType(keyType) && !context.typesController.isCustomType(keyType))
            {
                Message message;
                message.content = tr("Map key type %1 does not exist.").arg(keyType);
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
