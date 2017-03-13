#include "mapkeytypenotsupportedtask.h"

#include "../../Tasks/Model/taskcontext.h"
#include "../../Types/Controller/typescontroller.h"
#include "../../Types/Model/builtintype.h"

using namespace Tome;


const QString MapKeyTypeNotSupportedTask::MessageCode = "TO0105";


MapKeyTypeNotSupportedTask::MapKeyTypeNotSupportedTask()
{
}

const QString MapKeyTypeNotSupportedTask::getDisplayName() const
{
    return MessageCode + tr(" - Map Key Type Not Supported");
}

const MessageList MapKeyTypeNotSupportedTask::execute(const TaskContext& context) const
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

            // Check list item type.
            if (keyType == BuiltInType::Vector2I || keyType == BuiltInType::Vector2R ||
                keyType == BuiltInType::Vector3I || keyType == BuiltInType::Vector3R)
            {
                Message message;
                message.content = tr("The key type %1 of the map type %2 is not supported.").arg(keyType, type.name);
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
