#include "mapvaluetypenotsupportedtask.h"

#include "../../Tasks/Model/taskcontext.h"
#include "../../Types/Controller/typescontroller.h"
#include "../../Types/Model/builtintype.h"

using namespace Tome;


const QString MapValueTypeNotSupportedTask::MessageCode = "TO0106";


MapValueTypeNotSupportedTask::MapValueTypeNotSupportedTask()
{
}

const MessageList MapValueTypeNotSupportedTask::execute(const TaskContext& context) const
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

            // Check list item type.
            if (valueType == BuiltInType::Vector2I || valueType == BuiltInType::Vector2R ||
                valueType == BuiltInType::Vector3I || valueType == BuiltInType::Vector3R)
            {
                Message message;
                message.content = "Map value type " + valueType + " is not supported.";
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
